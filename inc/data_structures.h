#include <Eigen/Core>
#include <complex>

// Simple struct for passing complex signals
struct MonopulseSignals
{
    std::complex<double> sum = 0.0;
    std::complex<double> deltaAz = 0.0;
    std::complex<double> deltaEl = 0.0;
};

// Simple struct for angular errors
struct AngularErrors
{
    double azError = 0.0; // Radians or Degrees
    double elError = 0.0; // Radians or Degrees
};

// Simple struct for antenna commands
struct AntennaCommands
{
    double azRateCmd = 0.0; // Or position delta, Radians/s or Radians
    double elRateCmd = 0.0; // Or position delta, Radians/s or Radians
};

// Simple struct for antenna angles
struct AntennaAngles
{
    double azimuth = 0.0;   // Radians or Degrees
    double elevation = 0.0; // Radians or Degrees
};

// Using Eigen for positions/velocities
using PositionVector = Eigen::Vector3d; // e.g., (x, y, z) or (range, az, el)
using VelocityVector = Eigen::Vector3d;

struct TargetState
{
    PositionVector position;
    VelocityVector velocity; // Optional
};