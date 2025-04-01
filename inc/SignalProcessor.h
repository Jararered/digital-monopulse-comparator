#pragma once

#include "DataTypes.h"

#include <complex>
#include <deque> // For optional simple averaging/filtering

class SignalProcessor
{
public:
    // Parameters: Monopulse slope (Km), filter settings
    SignalProcessor(double monopulseSlopeKm);

    // Processes raw signals to estimate errors
    AngularErrors estimateErrors(const MonopulseSignals &signals);

    void setParameters(double monopulseSlopeKm /*, filter params */);
    AngularErrors getLastErrors() const; // Get last computed errors

private:
    double km; // Monopulse slope
    AngularErrors lastEstimatedErrors;

    // Optional: Filter state (e.g., previous estimates for simple averaging)
    // std::deque<AngularErrors> errorHistory;
    // int filterWindowSize = 5;
};