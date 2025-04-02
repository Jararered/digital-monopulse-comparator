#include "Monopulse.hpp"

#include <iostream>

int main()
{
    MonopulseRadar radar;

    std::cout << "Pulse Frequency: " << radar.pulse.pulseFrequency << " GHz" << std::endl;
    std::cout << "Pulse Period: " << radar.pulse.pulsePeriod << " seconds" << std::endl;
    std::cout << "Pulse Width: " << radar.pulse.pulseWidth << " microseconds" << std::endl;

    double maxRange = CalculateMaxRange(radar);
    std::cout << "Max Range: " << maxRange << " meters" << std::endl;
}
