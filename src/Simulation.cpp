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
void Simulation::run() {
    
    std::cout << "Starting simulation..." << std::endl;
    
    // main time loop
    for (size_t step = 0; step < steps; ++step) {

        // Emit particles
        auto new_particles = source.emit();
        particles.insert(particles.end(),
                        new_particles.begin(),
                        new_particles.end());

        // --- Electrostatic forces via Barnes-Hut ---
        // Build an octree from all living *charged* particles and compute
        // the Coulomb acceleration on every charged particle.
        // Neutral particles (neutrons) neither exert nor feel Coulomb forces.
        {
            // Count charged particles and find their bounding cube.
            int n_charged = 0;
            double lo =  1e30, hi = -1e30;
            for (const auto& p : particles) {
                if (!p.alive || p.electric_charge == 0) continue;
                ++n_charged;
                lo = std::min({lo, p.position.x, p.position.y, p.position.z});
                hi = std::max({hi, p.position.x, p.position.y, p.position.z});
            }

            if (n_charged >= 2) {
                double margin = std::max(1.0, (hi - lo) * 0.01);
                vector3<double> minVec{lo - margin, lo - margin, lo - margin};
                vector3<double> maxVec{hi + margin, hi + margin, hi + margin};

                Octree octree(minVec, maxVec);
                octree.set_theta(0.5f);

                for (const auto& p : particles)
                    if (p.alive && p.electric_charge != 0)
                        octree.add_particle(p);

                for (auto& p : particles) {
                    if (!p.alive || p.electric_charge == 0) continue;
                    octree.calc_force(p);
                    // calc_force stores the raw Coulomb force in p.acceleration.
                    // Convert to acceleration: a = F / m  (mass in proton-mass units).
                    if (p.mass > 0)
                        p.acceleration = p.acceleration / p.mass;
                }
            }
        }

        // Move particles
        std::vector<Particle> secondaries; // collect secondaries outside the loop

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

                if (p.alive && uniform01() < 0.10) {
                    double speed = std::sqrt(p.velocity * p.velocity);
                    p.velocity.x += speed * 0.2 * (uniform01() - 0.5);
                    p.velocity.y += speed * 0.2 * (uniform01() - 0.5);
                    p.velocity.z += speed * 0.2 * (uniform01() - 0.5);
                    // Re-normalise so the total speed *decreases* (5 % energy loss).
                    double new_speed = std::sqrt(p.velocity * p.velocity);
                    if (new_speed > 0) {
                        double scale = speed * 0.95 / new_speed;
                        p.velocity = p.velocity * scale;
                    }
                }

                if (p.alive && p.type != "photon" && uniform01() < 0.01) {
                    if (p.type == "proton") {
                        if (uniform01() < 0.5) {
                            // 50 %: 1 neutron + 1 photon
                            secondaries.emplace_back(p.position.x, p.position.y, p.position.z,
                                                     p.velocity.x, p.velocity.y, p.velocity.z, "neutron");
                            secondaries.emplace_back(p.position.x, p.position.y, p.position.z,
                                                     p.velocity.x, p.velocity.y, p.velocity.z, "photon");
                        } else {
                            // 50 %: 1 photon
                            secondaries.emplace_back(p.position.x, p.position.y, p.position.z,
                                                     p.velocity.x, p.velocity.y, p.velocity.z, "photon");
                        }
                    } else if (p.type == "neutron") {
                        if (uniform01() < 0.5) {
                            // 50 %: 1 proton + 1 neutron
                            secondaries.emplace_back(p.position.x, p.position.y, p.position.z,
                                                     p.velocity.x, p.velocity.y, p.velocity.z, "proton");
                            secondaries.emplace_back(p.position.x, p.position.y, p.position.z,
                                                     p.velocity.x, p.velocity.y, p.velocity.z, "neutron");
                        } else {
                            // 50 %: 2 photons
                            secondaries.emplace_back(p.position.x, p.position.y, p.position.z,
                                                     p.velocity.x, p.velocity.y, p.velocity.z, "photon");
                            secondaries.emplace_back(p.position.x, p.position.y, p.position.z,
                                                     p.velocity.x, p.velocity.y, p.velocity.z, "photon");
                        }
                    }
                }
            }

            roi.score(p);
        }

        // Append any secondaries generated this step
        particles.insert(particles.end(), secondaries.begin(), secondaries.end());

        // Remove particles that are dead from simulation
        // this makes use of Modern C++ algorithm and lambda function
        // Remove dead particles to keep vector size reasonable
        particles.erase(
            std::remove_if(particles.begin(), particles.end(),
                        [](const Particle& p) { return !p.alive; }),
            particles.end()
        );


        // Output every N steps
        if (step % 2 == 0) {
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