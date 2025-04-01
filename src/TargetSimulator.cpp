#include "TargetSimulator.h"

#include <stdexcept> // For potential error checking

TargetSimulator::TargetSimulator(const PositionVector &initialPos, const VelocityVector &initialVel)
    : currentState{initialPos, initialVel} {}

void TargetSimulator::update(double dt)
{
    if (dt < 0.0)
    {
        // Optional: Add error handling or clamping
        // throw std::runtime_error("Negative dt not allowed");
        dt = 0.0;
    }
    // linalg.h overloads operators+, -, *, / for vector-scalar and vector-vector ops
    currentState.position = currentState.position + currentState.velocity * dt;
}

TargetState TargetSimulator::getState() const
{
    return currentState;
}

void TargetSimulator::setVelocity(const VelocityVector &vel)
{
    currentState.velocity = vel;
}