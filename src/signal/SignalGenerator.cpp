#include "SignalGenerator.hpp"

#include <core/Constants.hpp>

#include <cmath>

double SignalGenerator::Update(double time)
{
    return m_Parameters.amplitude * sin(2 * Constants::PI * m_Parameters.frequency * time + m_Parameters.phase) + m_Parameters.offset;
}
