#pragma once

#include "Constants.hpp"
#include "Types.hpp"
#include "Simulation.hpp"

#include <vector>

struct PulseParameters
{
    double pulseAmplitude = 1.0;   // Volts (1V)
    double pulseFrequency = 2.8e9; // Hz (2.8GHz)
    double pulsePeriod = 1e-3;     // seconds (1ms)
    double pulseWidth = 1e-6;      // seconds (1us)
};

struct AntennaParameters
{
    Position position;
    Velocity velocity;
    double separation = 1.0; // meters
    double height = 1.0;     // meters
    double azimuth = 0.0;    // degrees
    double elevation = 0.0;  // degrees
};

struct MonopulseRadar
{
    AntennaParameters antenna;
    PulseParameters pulse;
    double pulseRepetitionFrequency = 1000.0; // Hz (1kHz)
};

struct Target
{
    Position position;
    Velocity velocity;
    double crossSection = 1.0; // square meters
};

double CalculateMaxRange(const MonopulseRadar &pulse);

// A structure used to store a fixed set of values of a signal, and when setting values
class SignalStorage : public SimulationObject
{
public:
    SignalStorage(int size) : size(size), signal(size, 0)
    {
    }

    // anon func to generate random number between 0 and 1000
    float generateRandomNumber()
    {
        return static_cast<float>(rand()) / RAND_MAX * 1000;
    };

    // anon function to generate a sine wave with input being time
    float generateSineWave(float time)
    {
        float amplitude = 10;
        float frequency = 1000;
        return amplitude * static_cast<float>(sin(frequency * time));
    };

    void Initialize() override
    {
        signal.resize(size, 0);
    }

    void Update(double dt) override
    {
        time += static_cast<float>(dt);
        addValue(generateSineWave(time));
    }

    void Finalize() override
    {
        signal.resize(size, 0);
    }

    void Reset() override
    {
        signal.resize(size, 0);
        index = 0;
    }

    void addValue(float value)
    {
        // if index is greater than the size of the signal, reset the index
        if (index >= signal.size())
        {
            index = 0;
        }

        signal[index] = value;
        index++;
    }

    const std::vector<float> &getSignal() const
    {
        return signal;
    }

private:
    std::vector<float> signal;
    float time = 0;
    int size = 0;
    int index = 0;
};