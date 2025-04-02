#pragma once

#include <vector>

struct SimulationParameters
{
    double simDt = 1 / 60;       // 60 Hz / 16.6667 ms
    double simStartTime = 0;     // sec (0 sec)
    double simEndTime = 10;      // sec (10 sec)
    int updateCountPerFrame = 1; // 1 update per frame
};

class SimulationObject
{
public:
    virtual void Initialize() = 0;
    virtual void Update(double dt) = 0;
    virtual void Finalize() = 0;
    virtual void Reset() = 0;
};

class Simulation
{
public:
    Simulation(const SimulationParameters &params) : m_SimParamsInitial(params), m_SimParamsCurrent(params) {}
    virtual ~Simulation() = default;

    // Simulation management
    virtual void Initialize();
    virtual void Reset();
    virtual void Finalize();

    // Simulation control
    virtual void Start();
    virtual void Update();
    virtual void Step();
    virtual void Stop();

    // Simulation Object Management
    virtual void AddObject(SimulationObject *object);
    virtual void RemoveObject(SimulationObject *object);
    virtual void ClearObjects();

    // Rendering
    virtual void RenderControls();

    virtual double &getSimulationDt() { return m_SimParamsCurrent.simDt; }
    virtual double &getSimulationStartTime() { return m_SimParamsCurrent.simStartTime; }
    virtual double &getSimulationEndTime() { return m_SimParamsCurrent.simEndTime; }
    virtual double &getSimulationTime() { return m_SimulationTime; }
    virtual int &getUpdateCountPerFrame() { return m_SimParamsCurrent.updateCountPerFrame; }

protected:
    SimulationParameters m_SimParamsInitial;
    SimulationParameters m_SimParamsCurrent;
    bool m_Running = false;
    double m_SimulationTime = 0.0;

    std::vector<SimulationObject *> m_Objects;
};