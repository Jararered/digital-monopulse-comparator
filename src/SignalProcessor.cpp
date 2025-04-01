#include "SignalProcessor.h"

#include <cmath> // For std::real, std::abs
#include <stdexcept>

SignalProcessor::SignalProcessor(double monopulseSlopeKm) : km(monopulseSlopeKm)
{
    if (km == 0.0)
    {
        // Slope cannot be zero for division
        throw std::runtime_error("Monopulse slope Km cannot be zero.");
    }
}

AngularErrors SignalProcessor::estimateErrors(const MonopulseSignals &signals)
{
    // Use a small epsilon to avoid division by zero or near-zero
    const double epsilon = 1e-9;

    if (std::abs(signals.sum) < epsilon)
    {
        // Sum signal too weak, cannot estimate reliably
        lastEstimatedErrors = {0.0, 0.0}; // Output zero error
    }
    else
    {
        // Calculate complex ratios
        std::complex<double> ratioAz = signals.deltaAz / signals.sum;
        std::complex<double> ratioEl = signals.deltaEl / signals.sum;

        // Estimate error: Error = Re{Delta / Sum} / Km
        // Assuming the monopulse setup provides error information in the real part.
        lastEstimatedErrors.azError = std::real(ratioAz) / km;
        lastEstimatedErrors.elError = std::real(ratioEl) / km;

        // Optional: Add filtering (e.g., simple low-pass or moving average) here
        // ... filter logic using lastEstimatedErrors and previous values ...
    }
    return lastEstimatedErrors;
}

AngularErrors SignalProcessor::getLastErrors() const
{
    return lastEstimatedErrors;
}

void SignalProcessor::setParameters(double monopulseSlopeKm)
{
    if (monopulseSlopeKm == 0.0)
    {
        // Handle error appropriately, maybe log a warning or throw
        // For now, just prevent setting to zero
        return;
    }
    this->km = monopulseSlopeKm;
}