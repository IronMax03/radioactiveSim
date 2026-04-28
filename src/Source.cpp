/// @file Source.cpp
/// @brief Implementation of the Source class methods.

#include "Source.h"
#include "utils.h"
#include <cmath>

/// @brief Construct a source at the specified position with the given emission rate.
/// @param x_    X-coordinate of the source (metres).
/// @param y_    Y-coordinate of the source (metres).
/// @param z_    Z-coordinate of the source (metres).
/// @param rate_ Number of particles to emit per time step.
Source::Source(double x_, double y_, double z_,
               int rate_)
    : x(x_), y(y_), z(z_),
      rate(rate_)
{}

/// @brief Emit one batch of particles for the current time step.
///        Direction angles (theta, phi) and speed norm are drawn from normal
///        distributions to produce a realistic, slightly spread-out beam.
///        Particle type is chosen randomly: 75 % proton, 25 % neutron.
/// @return A vector of @c rate newly created Particle objects, all originating
///         from the source position.
std::vector<Particle> Source::emit() const {
    std::vector<Particle> emitted;

    std::random_device rd{};
    std::mt19937 gen{rd()};
    std::normal_distribution<> angle_distr(0, M_PI/16);
    std::normal_distribution<> norm_distr(1,0.25);

    // rate is basically the number of particles to emit at this timestep
    for (int i = 0; i < rate; ++i) {

        // make the source noisy by cahnging the angle and the velocity of a new particle 
        double theta = angle_distr(gen);
        double phi = angle_distr(gen);
        double norm = abs(norm_distr(gen));

        double vx = norm*sin(theta);
        double vy = norm*cos(phi) * sin(phi);
        double vz = norm*cos(theta) * cos(phi);

        // 75% of the time : proton, 25% : neutron
        std::string particle_type = "proton";
        if (uniform01() > 1)
            particle_type = "neutron";

        // create a particle on the spot and store it
        emitted.emplace_back(
            x, y, z,
            vx, vy, vz,
            particle_type
        );
    }

    return emitted;
}
