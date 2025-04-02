#pragma once

#include <glad/gl.h>
#include <glfw/glfw3.h>

#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/imgui.h>

#include <cstdio>
#include <string>

#include "Simulation.hpp"

struct ApplicationParams
{
    std::string title = "Application";
    int width = 1280;
    int height = 720;
    bool vsync = true;
    bool darkmode = true;
    int glVersionMajor = 4;
    int glVersionMinor = 6;
    std::string glslVersion = "#version 460";
};

class Application
{
public:
    Application(const ApplicationParams &params) : m_AppParams(params) {}
    virtual ~Application() = default;

    virtual void Initialize();
    virtual void Update();
    virtual void Finalize();

    virtual void NewFrame();
    virtual void Render();
    virtual void EndFrame();

    virtual void SetSimulation(Simulation *simulation)
    {
        if (p_Simulation != nullptr)
        {
            fprintf(stderr, "Simulation is already set\n");
            return;
        }

        if (simulation == nullptr)
        {
            fprintf(stderr, "Simulation is nullptr\n");
            return;
        }

        p_Simulation = simulation;
    }

    virtual Simulation *GetSimulation()
    {
        if (p_Simulation == nullptr)
        {
            fprintf(stderr, "Simulation is not set\n");
            return nullptr;
        }
        return p_Simulation;
    }

    virtual void ClearSimulation()
    {
        if (p_Simulation == nullptr)
        {
            fprintf(stderr, "Simulation is not set\n");
            return;
        }

        delete p_Simulation;
        p_Simulation = nullptr;
    }

    bool isRunning() const { return glfwWindowShouldClose(m_Window) == 0; }

protected:
    GLFWwindow *m_Window = nullptr;

    ApplicationParams m_AppParams;
    Simulation *p_Simulation = nullptr;
};
