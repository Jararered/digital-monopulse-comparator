#include "Application.hpp"

static void glfw_error_callback(int error, const char *description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

void Application::Initialize()
{
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
    {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, m_AppParams.glVersionMajor);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, m_AppParams.glVersionMinor);

    // Create window with graphics context
    m_Window = glfwCreateWindow(m_AppParams.width, m_AppParams.height, m_AppParams.title.c_str(), nullptr, nullptr);
    if (m_Window == nullptr)
    {
        fprintf(stderr, "Failed to create GLFW window\n");
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
        fprintf(stderr, "Failed to initialize OpenGL context\n");
        return;
    }

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
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
        for (int i = 0; i < p_Simulation->getUpdateCountPerFrame(); i++)
        {
            p_Simulation->Update();
        }
    }
}

void Application::Finalize()
{
    // Cleanup
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
