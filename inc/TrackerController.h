#pragma once

#include "DataTypes.h"

class TrackerController
{
public:
    // Parameters: Controller gains (e.g., proportional gain)
    TrackerController(double proportionalGain);

    // Calculates commands based on errors
    AntennaCommands computeCommands(const AngularErrors &errors);

    void setParameters(double proportionalGain);
    AntennaCommands getLastCommands() const;

private:
    double k_p; // Proportional gain
    // Add state for PI/PID if needed (integral term, derivative term)
    AntennaCommands lastComputedCommands;
};