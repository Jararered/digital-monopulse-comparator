#include <stdio.h>
#include <vector>

#include "Application.hpp"
#include "Monopulse.hpp"
#include "Simulation.hpp"

int main(int argc, char **argv)
{
    ApplicationParams appParams;
    appParams.title = "Monopulse Tracker";
    Application app(appParams);
    app.Initialize();

    SimulationParameters simParams;
    simParams.simDt = 1e-9;       // 1 GHz
    simParams.simStartTime = 0.0; // 0 sec
    simParams.simEndTime = 1.0;   // 1 sec
    simParams.updateCountPerFrame = 1;
    app.SetSimulation(new Simulation(simParams));

    SignalStorage signalStorage(4096);
    app.GetSimulation()->AddObject(&signalStorage);

    // Main loop
    while (app.isRunning())
    {
        // Update application and simulation before rendering
        app.Update();

        // Start new frame
        app.NewFrame();

        ImGui::Begin("Signal");
        if (ImGui::CollapsingHeader("Signal Display"))
        {
            // Get region of imgui signal storage size, account for other widgets in the window
            ImVec2 signalStorageSize = ImGui::GetContentRegionAvail();

            // automatically resize the y axis to the max and min of the signal
            static float running_min = 0;
            static float running_max = 0;
            float min = *std::min_element(signalStorage.getSignal().begin(), signalStorage.getSignal().end());
            float max = *std::max_element(signalStorage.getSignal().begin(), signalStorage.getSignal().end());
            if (min < running_min)
            {
                running_min = min;
            }
            if (max > running_max)
            {
                running_max = max;
            }
            ImGui::PlotLines("##Signal", signalStorage.getSignal().data(), (int)signalStorage.getSignal().size(), 0, NULL, running_min, running_max, signalStorageSize);
        }
        ImGui::End();

        app.GetSimulation()->RenderControls();

        app.Render();
        app.EndFrame();
    }

    app.Finalize();

    return 0;
}