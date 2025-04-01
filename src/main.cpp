#include <stdio.h>
#include <vector>

#include <glad/gl.h>
#include <glfw/glfw3.h>

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>

#include <DataTypes.h>
#include <TargetSimulator.h>
#include <AntennaPlatform.h>
#include <SignalProcessor.h>
#include <TrackerController.h>

// Define PI if not available
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

static void glfw_error_callback(int error, const char *description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

// Helper function for plotting with ImGui (use float)
void PlotSignal(const char *label, const std::vector<float> &data)
{
    if (!data.empty())
    {
        // No cast needed now
        ImGui::PlotLines(label, data.data(), static_cast<int>(data.size()), 0, nullptr, -1.0f, 1.0f, ImVec2(0, 80)); // Use data.data() directly
        // Added default scale min/max - adjust as needed
    }
}

int main(int, char **)
{
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

    // GL 3.0 + GLSL 130
    const char *glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    // Create window with graphics context
    GLFWwindow *window = glfwCreateWindow(1280, 720, "Dear ImGui GLFW+OpenGL3 example", nullptr, nullptr);
    if (window == nullptr)
    {
        return 1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    int version = gladLoadGL(glfwGetProcAddress);
    if (version == 0)
    {
        printf("Failed to initialize OpenGL context\n");
        return -1;
    }

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Our state
    bool show_demo_window = true;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // --- Simulation Setup ---
    double simulationTime = 0.0;
    double dt = 0.01; // Simulation time step (seconds)

    // Initial conditions
    TargetState initialTargetState;
    initialTargetState.position = {10000.0, 1000.0, 500.0}; // Example position
    initialTargetState.velocity = {-50.0, 0.0, 10.0};       // Example velocity
    AntennaAngles initialAntennaAngles = {0.0, 0.0};        // Pointing origin initially

    // Instantiate modules
    TargetSimulator targetSim(initialTargetState.position, initialTargetState.velocity);
    AntennaPlatform antenna(initialAntennaAngles, 0.035); // ~2 degrees beamwidth
    SignalProcessor sigProc(2.0);                         // Example monopulse slope
    TrackerController controller(0.5);                    // Example proportional gain

    // Data storage for plotting (use float)
    std::vector<float> timeHistory;
    std::vector<float> targetAzHistory, targetElHistory;
    std::vector<float> antennaAzHistory, antennaElHistory;
    std::vector<float> errorAzHistory, errorElHistory;
    std::vector<float> cmdAzHistory, cmdElHistory;
    const int maxHistory = 500; // Store last 500 points

    bool isRunning = true;

    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        if (glfwGetWindowAttrib(window, GLFW_ICONIFIED) != 0)
        {
            ImGui_ImplGlfw_Sleep(10);
            continue;
        }

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // --- Simulation Step ---
        if (isRunning)
        {
            // 1. Update Target Position
            targetSim.update(dt);
            TargetState currentTarget = targetSim.getState();

            // 2. Get Commands from Controller (using errors from *previous* step)
            AngularErrors lastErrors = sigProc.getLastErrors();
            AntennaCommands commands = controller.computeCommands(lastErrors);

            // 3. Update Antenna Pointing
            antenna.updatePointing(commands, dt);
            AntennaAngles currentAntennaAngles = antenna.getCurrentAngles();

            // 4. Generate Monopulse Signals
            MonopulseSignals signals = antenna.generateSignals(currentTarget);

            // 5. Estimate Angular Errors
            AngularErrors currentErrors = sigProc.estimateErrors(signals);

            // --- Data Logging for Plots ---
            simulationTime += dt;
            timeHistory.push_back(static_cast<float>(simulationTime));
            // (Calculate true target Az/El relative to origin if needed for comparison)
            // targetAzHistory.push_back(...);
            // targetElHistory.push_back(...);
            antennaAzHistory.push_back(static_cast<float>(currentAntennaAngles.azimuth * 180.0 / M_PI));   // Cast results
            antennaElHistory.push_back(static_cast<float>(currentAntennaAngles.elevation * 180.0 / M_PI)); // Cast results
            errorAzHistory.push_back(static_cast<float>(currentErrors.azError * 180.0 / M_PI));            // Cast results
            errorElHistory.push_back(static_cast<float>(currentErrors.elError * 180.0 / M_PI));            // Cast results
            cmdAzHistory.push_back(static_cast<float>(commands.azRateCmd));                                // Cast results
            cmdElHistory.push_back(static_cast<float>(commands.elRateCmd));                                // Cast results

            // Limit history size
            auto limitHistory = [](std::vector<float> &vec, size_t maxSize)
            {
                if (vec.size() > maxSize)
                {
                    vec.erase(vec.begin(), vec.begin() + (vec.size() - maxSize));
                }
            };

            limitHistory(timeHistory, maxHistory);
            limitHistory(antennaAzHistory, maxHistory);
            limitHistory(antennaElHistory, maxHistory);
            limitHistory(errorAzHistory, maxHistory);
            limitHistory(errorElHistory, maxHistory);
            limitHistory(cmdAzHistory, maxHistory);
            limitHistory(cmdElHistory, maxHistory);

        } // end if(isRunning)

        // --- ImGui UI Definition ---
        ImGui::Begin("Monopulse Tracker Control");
        ImGui::Checkbox("Run Simulation", &isRunning);
        ImGui::SameLine();
        if (ImGui::Button("Reset"))
        {
            // TODO: Implement reset logic (re-initialize modules, clear history)
            simulationTime = 0.0;
            timeHistory.clear();
            // targetSim = TargetSimulator(...); // Re-create or add reset method
            // antenna = AntennaPlatform(...);
            // sigProc = SignalProcessor(...);
            // controller = TrackerController(...);
        }
        ImGui::InputDouble("Time Step (dt)", &dt, 0.001, 0.01, "%.4f");
        // Display Module Parameters & State
        if (ImGui::CollapsingHeader("Target"))
        {
            TargetState ts = targetSim.getState();

            ImGui::Text("Pos: (%.2f, %.2f, %.2f)", ts.position[0], ts.position[1], ts.position[2]);
            ImGui::Text("Vel: (%.2f, %.2f, %.2f)", ts.velocity[0], ts.velocity[1], ts.velocity[2]);

            float pos_arr[3] = {(float)ts.position[0], (float)ts.position[1], (float)ts.position[2]};
            ImGui::InputFloat3("Position (Display Only)", pos_arr, "%.1f", ImGuiInputTextFlags_ReadOnly); // Readonly example

            float vel_arr[3] = {(float)ts.velocity[0], (float)ts.velocity[1], (float)ts.velocity[2]};
            if (ImGui::InputFloat3("Velocity", vel_arr, "%.1f"))
            {
                targetSim.setVelocity({(double)vel_arr[0], (double)vel_arr[1], (double)vel_arr[2]});
            }
        }
        if (ImGui::CollapsingHeader("Antenna"))
        {
            AntennaAngles aa = antenna.getCurrentAngles();
            ImGui::Text("Azimuth: %.3f deg", aa.azimuth * 180.0 / M_PI);
            ImGui::Text("Elevation: %.3f deg", aa.elevation * 180.0 / M_PI);
            // Add antenna parameters if configurable
        }
        if (ImGui::CollapsingHeader("Signal Processor"))
        {
            AngularErrors ae = sigProc.getLastErrors();
            ImGui::Text("Est. Error Az: %.4f deg", ae.azError * 180.0 / M_PI);
            ImGui::Text("Est. Error El: %.4f deg", ae.elError * 180.0 / M_PI);
            // Add parameters (Km)
        }
        if (ImGui::CollapsingHeader("Controller"))
        {
            AntennaCommands ac = controller.getLastCommands();
            ImGui::Text("Cmd Rate Az: %.4f rad/s", ac.azRateCmd);
            ImGui::Text("Cmd Rate El: %.4f rad/s", ac.elRateCmd);
            // Add parameters (Gains)
        }
        ImGui::End(); // Control Window

        ImGui::Begin("Plots");
        PlotSignal("Antenna Az (deg)", antennaAzHistory);
        PlotSignal("Antenna El (deg)", antennaElHistory);
        PlotSignal("Error Az (deg)", errorAzHistory);
        PlotSignal("Error El (deg)", errorElHistory);
        PlotSignal("Command Az Rate", cmdAzHistory);
        PlotSignal("Command El Rate", cmdElHistory);
        ImGui::End(); // Plots Window

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}