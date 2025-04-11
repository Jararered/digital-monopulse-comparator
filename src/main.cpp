#include "core/Application.hpp"
#include "core/Simulation.hpp"

#include "signal/SignalDisplayObject.hpp"

#include <stdio.h>
#include <vector>

int main(int argc, char **argv)
{
    // Setup the Application
    ApplicationParams appParams;
    appParams.title = "Monopulse Tracker";
    Application app(appParams);
    app.Initialize();

    // Setup the Simulation
    SimulationParameters simParams;
    simParams.simTimeStep = 1e-9 * pow(2, 12); // 4096 ns
    simParams.simStartTime = 0.0;              // 0 sec
    simParams.updateCountPerFrame = 10;
    Simulation *simulation = app.CreateSimulation(simParams);

    // Add an object to the Simulation
    SignalDisplayObject signalDisplay(4096);
    simulation->AddObject(&signalDisplay);

    // Begins the applications main loop
    app.Start();

    return 0;
}