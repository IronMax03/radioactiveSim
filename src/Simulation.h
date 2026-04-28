/// @file Simulation.h
/// @brief Declaration of the Simulation class, which orchestrates the full
///        radioactive particle transport simulation.

#pragma once

#include <vector>
#include "Particle.h"
#include "Shielding.h"
#include "ROI.h"
#include "Source.h"

/// @brief Top-level simulation driver.
///
/// The Simulation class owns all physical objects (source, shielding, ROI) and
/// the list of live particles. Calling run() advances the simulation for the
/// configured number of time steps, handling emission, movement, shielding
/// absorption, ROI scoring, and periodic CSV output.
class Simulation {
public:
    /// @brief Time step duration in seconds.
    double dt;

    /// @brief Total number of time steps to simulate.
    unsigned steps;

    /// @brief The radioactive point source that emits particles each time step.
    Source source;

    /// @brief List of particles currently active in the simulation.
    std::vector<Particle> particles;

    /// @brief The shielding volume that attenuates particles passing through it.
    Shielding shielding;

    /// @brief The region of interest used to score particles.
    ROI roi;

    /// @brief Construct a simulation with all required physical parameters.
    /// @param dt    Time step duration (seconds).
    /// @param steps Number of time steps to run.
    /// @param sr    Radioactive source configuration.
    /// @param s     Shielding volume and material.
    /// @param r     Region of interest for particle scoring.
    Simulation(double dt, unsigned steps, Source sr,
               Shielding s, ROI r);

    /// @brief Execute the simulation for @c steps time steps.
    ///        Each step: emits new particles, moves all live particles, handles
    ///        shielding absorption, scores the ROI, and writes periodic CSV output.
    void run();

    /// @brief Print a human-readable summary of the simulation parameters to @c stdout.
    void print();
};
