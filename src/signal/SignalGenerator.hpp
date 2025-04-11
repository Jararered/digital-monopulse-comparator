#pragma once

#include "SignalParameters.hpp"

class SignalGenerator
{
public:
    SignalGenerator(const SignalParameters &parameters) : m_Parameters(parameters) {}
    virtual ~SignalGenerator() = default;

    virtual double Update(double time) = 0;

private:
    SignalParameters m_Parameters;
};
