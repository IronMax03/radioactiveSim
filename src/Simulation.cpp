/// @file Simulation.cpp
/// @brief Implementation of the Simulation class methods.

#include "Simulation.h"
#include "utils.h"
#include "Output.h"
#include <cmath>
#include <iostream>
#include <algorithm>

/// @brief Construct a Simulation with the given physical parameters.
/// @param dt_    Time step duration (seconds).
/// @param steps_ Total number of time steps to run.
/// @param src    Radioactive source configuration.
Simulation::Simulation(double dt_, unsigned steps_)
    : dt(dt_), steps(steps_)
{}

/// @brief Execute the simulation for the configured number of time steps.
void Simulation::run() 
{
    
    std::cout << "Starting simulation..." << std::endl;
    
    for (size_t step = 0; step < steps; ++step) 
    {

        // Emit one particle with random bounded position and velocity
        {
            Particle p;
            // position: around the source within +/-0.5 units
            double spread_pos = 0.5;
            p.position.x = (uniform01() - 0.5) * 2.0 * spread_pos;
            p.position.y = (uniform01() - 0.5) * 2.0 * spread_pos;
            p.position.z = (uniform01() - 0.5) * 2.0 * spread_pos;

            // velocity: random direction with magnitude up to 1.0 units/sec
            double speed = 1.0 * uniform01();
            p.velocity.x = (uniform01() - 0.5) * 2.0;
            p.velocity.y = (uniform01() - 0.5) * 2.0;
            p.velocity.z = (uniform01() - 0.5) * 2.0;
            // normalize direction then scale by speed
            double vmag = std::sqrt(p.velocity * p.velocity);
            if (vmag > 0) {
                double scale = speed / vmag;
                p.velocity = p.velocity * scale;
            }

            // basic defaults
            p.alive = true;
            p.electric_charge = 1.0;
            p.mass = 1.0;
            p.distance_in_shield = 0.0;
            p.acceleration = vector3<double>{0.0, 0.0, 0.0};

            particles.push_back(p);
        }


        // Count charged particles and find their bounding cube.
        int n_charged = 0;
        double lo =  1e30, hi = -1e30;
        for (const auto& p : particles) 
        {
            if (!p.alive || p.electric_charge == 0) continue;
                ++n_charged;
                lo = std::min({lo, p.position.x, p.position.y, p.position.z});
                hi = std::max({hi, p.position.x, p.position.y, p.position.z});
        }

        if (n_charged >= 2) 
        {
            double margin = std::max(1.0, (hi - lo) * 0.01);
            vector3<double> minVec{lo - margin, lo - margin, lo - margin};
            vector3<double> maxVec{hi + margin, hi + margin, hi + margin};

            Octree octree(minVec, maxVec);
            octree.set_theta(0.5f);

            for (const auto& p : particles)
                if (p.alive && p.electric_charge != 0)
                    octree.add_particle(p);

            for (auto& p : particles) 
            {
                if (!p.alive || p.electric_charge == 0) continue;
                    octree.calc_force(p);

                    if (p.mass > 0)
                        p.acceleration = p.acceleration / p.mass;
            }
        }


        for (auto& p : particles) 
        {
            if (!p.alive) continue;

            p.move(dt);
            //is_in_bounding_box(p); //! new
        }

        particles.erase(
            std::remove_if(particles.begin(), particles.end(),
                        [](const Particle& p) { return !p.alive; }),
            particles.end()
        );


        if (step % 2 == 0)
            write_particles_csv(particles, "output/particles_" + std::to_string(step) + ".csv");

        if (step % (steps / 10) == 0) 
            std::cout << "  Progress: " << (100 * step) / steps << "% " << std::endl;
    }
    std::cout << "Done." << std::endl;
}

/// @brief Print a human-readable summary of the simulation parameters to @c stdout.
///        Includes time step, number of steps, total simulated time, source position
///        and rate, shielding centre and material, and ROI centre.
void Simulation::print() 
{
    std::cout << "--- Simulation parameters ---" << std::endl;
    std::cout << "| dt = " << dt << ", number of steps = " << steps << " => total time = " << steps * dt << std::endl;
    std::cout << "-----------------------------" << std::endl;
}