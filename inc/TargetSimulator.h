#pragma once

#include "DataTypes.h" // Include the structures defined above

class TargetSimulator
{
public:
    TargetSimulator(const PositionVector &initialPos, const VelocityVector &initialVel);

    void update(double dt); // Updates position based on velocity and dt
    TargetState getState() const;
    void setVelocity(const VelocityVector &vel); // Allow changing trajectory

private:
    TargetState currentState;
};