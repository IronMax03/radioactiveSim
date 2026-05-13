/// @file Simulation.h
/// @brief Declaration of the Simulation class, which orchestrates the full
///        radioactive particle transport simulation.

#pragma once

#include <vector>
#include "Particle.h"
#include "Shielding.h"
#include "ROI.h"
#include "Source.h"
#include "Octree.h"

/// @brief Top-level simulation driver.
///
/// The Simulation class owns all physical objects (source, shielding, ROI) and
/// the list of live particles. Calling run() advances the simulation for the
/// configured number of time steps, handling emission, movement, shielding
/// absorption, ROI scoring, and periodic CSV output.
class Simulation 
{
public:
    /// @brief Time step duration in seconds.
    double dt;

    /// @brief Total number of time steps to simulate.
    unsigned steps;

    /// @brief List of particles currently active in the simulation.
    std::vector<Particle> partic les;

    /// @brief Construct a simulation with all required physical parameters.
    /// @param dt    Time step duration (seconds).
    /// @param steps Number of time steps to run.
    Simulation(double dt, unsigned steps);

    /// @brief Execute the simulation for @c steps time steps.
    ///        Each step: emits new particles, moves all live particles, handles
    ///        shielding absorption, scores the ROI, and writes periodic CSV output.
    void run();

    /// @brief Print a human-readable summary of the simulation parameters to @c stdout.
    void print();
};