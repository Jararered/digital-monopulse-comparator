#include "TrackerController.h"

#include <stdexcept>

TrackerController::TrackerController(double proportionalGain) : k_p(proportionalGain)
{
    if (proportionalGain < 0)
    {
        // Typically gain is positive
        // throw std::runtime_error("Controller gain should be non-negative");
    }
}

AntennaCommands TrackerController::computeCommands(const AngularErrors &errors)
{
    // Simple Proportional Controller
    lastComputedCommands.azRateCmd = k_p * errors.azError;
    lastComputedCommands.elRateCmd = k_p * errors.elError;

    // Optional: Add command limits (max angular rate)
    // double maxRate = 0.5; // Radians/sec
    // lastComputedCommands.azRateCmd = std::max(-maxRate, std::min(maxRate, lastComputedCommands.azRateCmd));
    // lastComputedCommands.elRateCmd = std::max(-maxRate, std::min(maxRate, lastComputedCommands.elRateCmd));

    return lastComputedCommands;
}

AntennaCommands TrackerController::getLastCommands() const
{
    return lastComputedCommands;
}

void TrackerController::setParameters(double proportionalGain)
{
    // Add validation if needed
    this->k_p = proportionalGain;
}