#include "Monopulse.hpp"

#include <iostream>

double CalculateMaxRange(const MonopulseRadar &radar)
{
    double deadTimeBetweenPulses = 1.0 / radar.pulseRepetitionFrequency - radar.pulse.pulseWidth;
    double maxRange = Constants::C * deadTimeBetweenPulses / 2;
    return maxRange;
}
