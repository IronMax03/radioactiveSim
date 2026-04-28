#pragma once

#include <vector>
#include "Particle.h"
#include "Shielding.h"
#include "ROI.h"
#include "Source.h"

class Simulation {
public:
    double dt;
    unsigned steps;

    Source source;
    std::vector<Particle> particles;
    Shielding shielding;
    ROI roi;

    Simulation(double dt, unsigned steps, Source sr,
               Shielding s, ROI r);

    void run();
    void print();
};
