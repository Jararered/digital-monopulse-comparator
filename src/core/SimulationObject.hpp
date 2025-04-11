#pragma once

#include <implot.h>

#include <cmath>
#include <string>
#include <vector>

class SimulationObject
{
public:
    // Required functions to implement
    virtual void Initialize() = 0;
    virtual void Update(double dt) = 0;
    virtual void Render() = 0;
    virtual void Finalize() = 0;
    virtual void Reset() = 0;
};

// This is here for convenience to create a new SimulationObject
// SimulationObjectTemplate.hpp
class SimulationObjectTemplate : public SimulationObject
{
public:
    void Initialize() override;
    void Update(double dt) override;
    void Render() override;
    void Finalize() override;
    void Reset() override;
};

// SimulationObjectTemplate.cpp
/*
#include "SimulationObjectTemplate.hpp"

void SimulationObjectTemplate::Initialize()
{
}

void SimulationObjectTemplate::Update(double dt)
{
}

void SimulationObjectTemplate::Render()
{
}

void SimulationObjectTemplate::Finalize()
{
}

void SimulationObjectTemplate::Reset()
{
}
*/
