/// @file Simulation.cpp
/// @brief Implementation of the Simulation class methods.

#include "Simulation.h"
#include "utils.h"
#include "Output.h"
#include "Source.h"
#include "GeometryOutput.h"
#include <cmath>
#include <iostream>
#include <algorithm>

/// @brief Construct a Simulation with the given physical parameters.
/// @param dt_    Time step duration (seconds).
/// @param steps_ Total number of time steps to run.
/// @param src    Radioactive source configuration.
/// @param s      Shielding volume and material.
/// @param r      Region of interest for scoring particles.
Simulation::Simulation(double dt_, unsigned steps_, Source src,
                       Shielding s, ROI r)
    : dt(dt_), steps(steps_), source(src),
      shielding(s), roi(r)
{}

/// @brief Execute the simulation for the configured number of time steps.
///
/// Each step:
/// -# Emits new particles from the source and appends them to the particle list.
/// -# Moves every live particle by @c dt using its current kinematics.
/// -# Checks whether each particle is still inside the simulation bounding box.
/// -# Applies exponential attenuation to particles inside the shielding volume.
/// -# Scores particles that have entered the ROI.
/// -# Removes dead particles from the list to keep memory usage bounded.
/// -# Writes a CSV snapshot every 10 steps to the @c output/ directory.
/// -# Prints progress to @c stdout every 10 % of total steps.
void Simulation::run() {
    
    std::cout << "Starting simulation..." << std::endl;
    
    // main time loop
    for (size_t step = 0; step < steps; ++step) {

        // Emit particles
        auto new_particles = source.emit();
        particles.insert(particles.end(),
                        new_particles.begin(),
                        new_particles.end());

        // Move particles
        for (auto& p : particles) {
            if (!p.alive) continue;

            p.move(dt);
            is_in_bounding_box(p); //! new

            // handle absorbtion of particle by shielding
            if (shielding.contains(p)) {
                p.distance_in_shield += dt * std::sqrt(p.velocity * p.velocity);

                double survival = std::exp(-p.distance_in_shield / shielding.material.mean_free_path);
                if (uniform01() > survival) {
                    p.alive = false;
                }
            }

            roi.score(p);
        }

        // Remove particles that are dead from simulation
        // this makes use of Modern C++ algorithm and lambda function
        // Remove dead particles to keep vector size reasonable
        particles.erase(
            std::remove_if(particles.begin(), particles.end(),
                        [](const Particle& p) { return !p.alive; }),
            particles.end()
        );


        // Output every N steps
        if (step % 10 == 0) {
            write_particles_csv(particles, "output/particles_" + std::to_string(step) + ".csv");
        }

        // Print message every 10% achieved
        if (step % (steps / 10) == 0) {
            std::cout << "  Progress: " << (100 * step) / steps << "% : # particles scored in ROI: " << roi.count << std::endl;
        }
    }
    std::cout << "Done." << std::endl;
}

/// @brief Print a human-readable summary of the simulation parameters to @c stdout.
///        Includes time step, number of steps, total simulated time, source position
///        and rate, shielding centre and material, and ROI centre.
void Simulation::print() {
    std::cout << "--- Simulation parameters ---" << std::endl;
    std::cout << "| dt = " << dt << ", number of steps = " << steps << " => total time = " << steps * dt << std::endl;
    std::cout << "| Source at ("<< source.x<<","<< source.y<<","<< source.z<<"), rate = "<< source.rate << " particles/dt = "<< source.rate/dt << " particles/second" << std::endl;
    std::cout << "| Shielding at "<< (shielding.min_corner + shielding.max_corner) / 2 << ", made of "<< shielding.material.name << std::endl;
    std::cout << "| ROI at "<< (roi.min_corner + roi.max_corner) / 2 << std::endl;
    std::cout << "-----------------------------" << std::endl;
}
