#include "Application.hpp"

static void glfw_error_callback(int error, const char *description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

void Application::Start()
{
    // Main loop
    while (isRunning())
    {
        // Start new frame
        NewFrame();

        // Update application and simulation before rendering
        Update();

        // Render the application and simulation
        Render();

        // End the frame
        EndFrame();
    }

    // While loop exits due to user closing the window
    Finalize();
}

void Application::Initialize()
{
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
    {
        fprintf(stderr, "Application::Initialize: Failed to initialize GLFW\n");
        return;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, m_AppParams.glVersionMajor);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, m_AppParams.glVersionMinor);

    // Create window with graphics context
    m_Window = glfwCreateWindow(m_AppParams.width, m_AppParams.height, m_AppParams.title.c_str(), nullptr, nullptr);
    if (m_Window == nullptr)
    {
        fprintf(stderr, "Application::Initialize: Failed to create GLFW window\n");
        return;
    }

    glfwMakeContextCurrent(m_Window);

    if (m_AppParams.vsync)
    {
        glfwSwapInterval(1); // Enable vsync
    }
    else
    {
        glfwSwapInterval(0); // Disable vsync
    }

    int version = gladLoadGL(glfwGetProcAddress);
    if (version == 0)
    {
        fprintf(stderr, "Application::Initialize: Failed to initialize OpenGL context\n");
        return;
    }

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImPlot::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls

    // Setup Dear ImGui style
    if (m_AppParams.darkmode)
    {
        ImGui::StyleColorsDark();
    }
    else
    {
        ImGui::StyleColorsLight();
    }

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(m_Window, true);
    ImGui_ImplOpenGL3_Init(m_AppParams.glslVersion.c_str());
}

void Application::Update()
{
    if (p_Simulation != nullptr)
    {
        p_Simulation->RenderControls();

        for (int i = 0; i < p_Simulation->getUpdateCountPerFrame(); i++)
        {
            p_Simulation->Update();
        }
        p_Simulation->RenderObjects();
    }
}

void Application::Finalize()
{
    // Cleanup
    ImPlot::DestroyContext();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(m_Window);
    glfwTerminate();
}

void Application::NewFrame()
{
    // Poll events
    glfwPollEvents();

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void Application::Render()
{
    // Rendering
    ImGui::Render();

    // Get these values from the window in case of window resize
    int display_w, display_h;
    glfwGetFramebufferSize(m_Window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);

    glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
    glClear(GL_COLOR_BUFFER_BIT);

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Application::EndFrame()
{
    // Swap buffers
    glfwSwapBuffers(m_Window);
}

Simulation *Application::CreateSimulation(SimulationParameters &params)
{
    if (p_Simulation != nullptr)
    {
        fprintf(stderr, "Application::CreateSimulation: Simulation is already set\n");
        return nullptr;
    }

    p_Simulation = new Simulation(params);
    return p_Simulation;
}

void Application::SetSimulation(Simulation *simulation)
{
    if (p_Simulation != nullptr)
    {
        fprintf(stderr, "Application::SetSimulation: Simulation is already set\n");
        return;
    }

    if (simulation == nullptr)
    {
        fprintf(stderr, "Application::SetSimulation: Simulation is nullptr\n");
        return;
    }

    p_Simulation = simulation;
}

Simulation *Application::GetSimulation()
{
    if (p_Simulation == nullptr)
    {
        fprintf(stderr, "Application::GetSimulation: Simulation is not set\n");
        return nullptr;
    }
    return p_Simulation;
}

void Application::ClearSimulation()
{
    if (p_Simulation == nullptr)
    {
        fprintf(stderr, "Application::ClearSimulation: Simulation is not set\n");
        return;
    }

    delete p_Simulation;
    p_Simulation = nullptr;
}

bool Application::isRunning() const { return glfwWindowShouldClose(m_Window) == 0; }