#ifndef PARTICLE_H
#define PARTICLE_H

#include "utils.h"

#include <string>

class Particle {
public:
    vector3<double> position; //double x, y, z;
    vector3<double> velocity; //double vx, vy, vz;
    int electric_charge; // ! new, coulomb units
    std::string type;
    double distance_in_shield;
    bool alive;

    Particle(double x, double y, double z,
             double vx, double vy, double vz,
             std::string type);

    Particle(double x, double y, double z,
             double vx, double vy, double vz,
             int ec,
             std::string type);

    void move(double dt);

    bool operator==(const Particle& p);
};

#endif
