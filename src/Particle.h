#ifndef PARTICLE_H
#define PARTICLE_H

#include <string>

class Particle {
public:
    double x, y, z;
    double vx, vy, vz;
    int electric_charge; // ! new, coulomb unit
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
