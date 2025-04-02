#include "Simulation.hpp"

#include "imgui.h"

#include <stdexcept>

// Simulation management
void Simulation::Initialize()
{
    m_Running = false;

    // Validate simulation parameters
    if (m_SimParamsCurrent.simStartTime >= m_SimParamsCurrent.simEndTime)
    {
        throw std::invalid_argument("Simulation start time must be less than end time");
    }
    if (m_SimParamsCurrent.simDt <= 0)
    {
        throw std::invalid_argument("Simulation time step must be greater than 0");
    }
}

void Simulation::Reset()
{
    m_Running = false;
    m_SimParamsCurrent = m_SimParamsInitial;
    m_SimulationTime = m_SimParamsCurrent.simStartTime;
}

void Simulation::Finalize()
{
    m_Running = false;
}

// Simulation control
void Simulation::Start()
{
    m_Running = true;
}

void Simulation::Update()
{
    // Only allow updating if the simulation is running
    if (!m_Running)
    {
        return;
    }

    // Advance the simulation time
    m_SimulationTime += m_SimParamsCurrent.simDt;

    // Update all objects
    for (auto object : m_Objects)
    {
        object->Update(m_SimParamsCurrent.simDt);
    }
}

void Simulation::Step()
{
    // Only allow stepping if the simulation is not running
    if (m_Running)
    {
        return;
    }

    // Advance the simulation time
    m_SimulationTime += m_SimParamsCurrent.simDt;

    // Update all objects
    for (auto object : m_Objects)
    {
        object->Update(m_SimParamsCurrent.simDt);
    }
}

void Simulation::Stop()
{
    m_Running = false;
}

// Simulation Object Management
void Simulation::AddObject(SimulationObject *object)
{
    m_Objects.push_back(object);
}

void Simulation::RemoveObject(SimulationObject *object)
{
    m_Objects.erase(std::remove(m_Objects.begin(), m_Objects.end(), object), m_Objects.end());
}

void Simulation::ClearObjects()
{
    m_Objects.clear();
}

void Simulation::RenderControls()
{
    ImGui::Begin("Simulation Controls");

    if (ImGui::CollapsingHeader("Simulation Parameters"))
    {
        ImGui::Text("Time Step: %.2f ns", getSimulationDt() * 1e9);
        if (ImGui::Button("+"))
        {
            getSimulationDt() *= 2;
        }
        ImGui::SameLine();
        if (ImGui::Button("-"))
        {
            getSimulationDt() /= 2;
        }

        // plus and minus buttons to increment/decrement update count per frame
        ImGui::Text("Update Count Per Frame: %d", getUpdateCountPerFrame());
        ImGui::InputInt("##updateCountPerFrame", &getUpdateCountPerFrame(), 1, 5);

        ImGui::Text("Start Time: %.2f ns", getSimulationStartTime() * 1e9);
        ImGui::Text("End Time: %.2f ns", getSimulationEndTime() * 1e9);
    }

    // Display simulation control
    if (ImGui::CollapsingHeader("Simulation Control"))
    {
        if (ImGui::Button("Start"))
        {
            Start();
        }
        ImGui::SameLine();
        if (ImGui::Button("Reset"))
        {
            Reset();
        }
        ImGui::SameLine();
        if (ImGui::Button("Stop"))
        {
            Stop();
        }
    }

    // Display simulation time in nanoseconds
    ImGui::Text("Simulation Time: %.2f ns", getSimulationTime() * 1e9);

    ImGui::End();
}
