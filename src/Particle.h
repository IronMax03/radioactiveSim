#pragma once

#include "utils.h"

#include <string>

class Particle {
public:
    vector3<double> position;
    vector3<double> velocity;
    vector3<double> acceleration;
    double mass; // ! new, units of proton mass
    int electric_charge; // ! new, coulomb units
    std::string type;
    double distance_in_shield;
    bool alive;

    Particle();

    Particle(double x, double y, double z,
             double vx, double vy, double vz,
             std::string type);

    Particle(double x, double y, double z,
             double vx, double vy, double vz,
             double m,
             int ec,
             std::string type);

    void move(double dt);
};
