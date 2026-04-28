#pragma once

#include <vector>
#include <string>
#include "Particle.h"

class Source {
public:
    // Source position
    double x, y, z;

    // Emission rate (particles per timestep)
    int rate;

    Source(double x, double y, double z,
           int rate);

    // Emit particles for one timestep
    std::vector<Particle> emit() const;
};
