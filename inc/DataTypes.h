// --- DataTypes.h ---
#pragma once

#include <complex>

#include "linalg.h"

// Use linalg types for vectors
using PositionVector = linalg::vec<double, 3>;
using VelocityVector = linalg::vec<double, 3>;
// If you needed matrices later: using RotationMatrix = linalg::mat<double, 3, 3>;

// Simple struct for passing complex signals (remains the same)
struct MonopulseSignals
{
    std::complex<double> sum = 0.0;
    std::complex<double> deltaAz = 0.0;
    std::complex<double> deltaEl = 0.0;
};

// Simple struct for angular errors (remains the same)
struct AngularErrors
{
    double azError = 0.0; // Radians
    double elError = 0.0; // Radians
};

// Simple struct for antenna commands (remains the same)
struct AntennaCommands
{
    double azRateCmd = 0.0; // Radians/s
    double elRateCmd = 0.0; // Radians/s
};

// Simple struct for antenna angles (remains the same)
struct AntennaAngles
{
    double azimuth = 0.0;   // Radians
    double elevation = 0.0; // Radians
};

// Target state now uses linalg vectors
struct TargetState
{
    PositionVector position;
    VelocityVector velocity;
};