#include "Source.h"
#include "utils.h"

Source::Source(double x_, double y_, double z_,
               int rate_)
    : x(x_), y(y_), z(z_),
      rate(rate_)
{}

std::vector<Particle> Source::emit() const {
    std::vector<Particle> emitted;
    // rate is basically the number of particles to emit at this timestep
    for (int i = 0; i < rate; ++i) {

        // Simple mono-directional source (along +z)
        double vx = 0.0;
        double vy = 0.0;
        double vz = 1.0;

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
