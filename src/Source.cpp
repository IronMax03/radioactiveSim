#include "Source.h"
#include "utils.h"
#include <cmath>

Source::Source(double x_, double y_, double z_,
               int rate_)
    : x(x_), y(y_), z(z_),
      rate(rate_)
{}

std::vector<Particle> Source::emit() const {
    std::vector<Particle> emitted;

    std::random_device rd{};
    std::mt19937 gen{rd()};
    std::normal_distribution<> distr(0, M_PI/16);

    // rate is basically the number of particles to emit at this timestep
    for (int i = 0; i < rate; ++i) {
        double theta = distr(gen);
        double phi = distr(gen);

        double vx = sin(theta);
        double vy = cos(phi) * sin(phi);
        double vz = cos(theta) * cos(phi);

        // 75% of the time : proton, 25% : neutron
        std::string particle_type = "proton";
        if (uniform01() > 0.75)
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
