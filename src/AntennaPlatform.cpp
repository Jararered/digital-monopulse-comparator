#include "AntennaPlatform.h"

#include <cmath> // For trig functions, sqrt, etc.
#include <stdexcept>

// Define PI if not available
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

AntennaPlatform::AntennaPlatform(const AntennaAngles &initialAngles, double beamwidthRad)
    : currentAngles(initialAngles), beamwidth(beamwidthRad)
{
    if (beamwidth <= 0)
    {
        throw std::runtime_error("Beamwidth must be positive");
    }
}

void AntennaPlatform::updatePointing(const AntennaCommands &commands, double dt)
{
    currentAngles.azimuth += commands.azRateCmd * dt;
    currentAngles.elevation += commands.elRateCmd * dt;

    // Optional: Add angle wrapping (e.g., keep azimuth in [-pi, pi])
    // currentAngles.azimuth = std::fmod(currentAngles.azimuth + M_PI, 2.0 * M_PI) - M_PI;
    // Optional: Clamp elevation (e.g., to [-pi/2, pi/2])
    // currentAngles.elevation = std::max(-M_PI / 2.0, std::min(M_PI / 2.0, currentAngles.elevation));
}

AntennaAngles AntennaPlatform::getCurrentAngles() const
{
    return currentAngles;
}

// --- generateSignals: Key changes here ---
MonopulseSignals AntennaPlatform::generateSignals(const TargetState &targetState)
{
    // Assumptions:
    // 1. Antenna is at the origin (0,0,0). If not, subtract antenna position from targetState.position.
    // 2. Antenna points along the direction defined by currentAngles.azimuth and currentAngles.elevation.
    // 3. We use a simple model where error is proportional to angular offset.

    PositionVector targetPos = targetState.position;

    // Calculate the vector representing the antenna's boresight direction
    double cosEl = std::cos(currentAngles.elevation);
    PositionVector boresightVector = {
        cosEl * std::cos(currentAngles.azimuth), // x
        cosEl * std::sin(currentAngles.azimuth), // y
        std::sin(currentAngles.elevation)        // z
    };
    // Ensure it's normalized (should be by construction, but good practice)
    boresightVector = linalg::normalize(boresightVector);

    // --- Calculate target direction vector and range ---
    double targetRange = linalg::length(targetPos);
    if (targetRange < 1e-6)
    { // Avoid division by zero if target is at origin
        return MonopulseSignals{0.0, 0.0, 0.0};
    }
    PositionVector targetDirection = targetPos / targetRange; // Normalize

    // --- Calculate angular separation between boresight and target direction ---
    // Use dot product: dot(a, b) = |a|*|b|*cos(theta) = cos(theta) for unit vectors
    double cosAngleBetween = linalg::dot(boresightVector, targetDirection);
    // Clamp due to potential numerical inaccuracies
    cosAngleBetween = std::max(-1.0, std::min(1.0, cosAngleBetween));
    double angleOffBoresight = std::acos(cosAngleBetween);

    // --- Simplified Antenna Pattern Simulation ---
    // Model gain drop-off based on angle off boresight (e.g., Gaussian)
    double sigma = beamwidth / (2.0 * std::sqrt(2.0 * std::log(2.0))); // Approx. sigma for FWHM beamwidth
    double sumGain = std::exp(-0.5 * std::pow(angleOffBoresight / sigma, 2.0));

    // Simulate complex sum signal (amplitude based on gain, arbitrary phase for now)
    std::complex<double> sumSignal = sumGain; // Assuming reference phase is 0

    // --- Calculate Relative Azimuth and Elevation Errors ---
    // This is the trickiest part without a full geometry library.
    // We need to project the target direction onto the antenna's local Az/El plane.
    // Method: Find vectors representing "local up" and "local right" for the antenna.

    // Basic "world up" vector
    PositionVector worldUp = {0.0, 0.0, 1.0};

    // Calculate local "right" vector using cross product
    // Handle poles: if boresight is near pure up/down, choose a different 'up'
    PositionVector localRight;
    if (std::abs(linalg::dot(boresightVector, worldUp)) < 0.999)
    { // Check if not pointing straight up/down
        localRight = linalg::normalize(linalg::cross(boresightVector, worldUp));
    }
    else
    {
        // Pointing near a pole, use world 'forward' (e.g., +X) to define 'right'
        PositionVector worldForward = {1.0, 0.0, 0.0};
        localRight = linalg::normalize(linalg::cross(boresightVector, worldForward));
    }

    // Calculate local "up" vector
    PositionVector localUp = linalg::normalize(linalg::cross(localRight, boresightVector));

    // Project the target *direction vector* onto the local right and up axes
    // The result approximates the tangent of the relative az/el angles for small angles
    // Or, more accurately, use the *difference vector* between target and boresight directions
    PositionVector diffVec = targetDirection - boresightVector;
    double approxRelAz = linalg::dot(diffVec, localRight); // Component along 'right'
    double approxRelEl = linalg::dot(diffVec, localUp);    // Component along 'up'

    // Convert these projections to approximate angles (small angle approximation: angle ~ projection)
    // A more precise way involves atan2 on projected components, but this is simpler.
    double relAzError = approxRelAz; // In radians (approximation)
    double relElError = approxRelEl; // In radians (approximation)

    // --- Simulate Difference Signals ---
    // Simple model: Delta signal is Sum * K * error_angle
    // K is related to monopulse slope Km, let's incorporate it in SignalProcessor
    // For phase: often DeltaAz is ~real for az errors, DeltaEl is ~real for el errors
    // Or sometimes they are in quadrature depending on the feed design. Let's assume real for simplicity.
    double sensitivity = 2.0 / beamwidth; // Heuristic: error signal strength scales inversely with beamwidth
    std::complex<double> deltaAzSignal = sumSignal * sensitivity * relAzError;
    std::complex<double> deltaElSignal = sumSignal * sensitivity * relElError;

    MonopulseSignals signals;
    signals.sum = sumSignal;
    signals.deltaAz = deltaAzSignal;
    signals.deltaEl = deltaElSignal;

    // Optional: Add simulated noise here
    // double noiseLevel = 0.01;
    // signals.sum += std::complex<double>(random_normal(0, noiseLevel), random_normal(0, noiseLevel));
    // ... add noise to delta channels ...

    return signals;
}

// Helper to simulate antenna pattern (could be more sophisticated)
// std::complex<double> AntennaPlatform::calculateSignal(...) { ... } // Placeholder if needed