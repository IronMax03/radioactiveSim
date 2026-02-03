#ifndef PARTICLE_H
#define PARTICLE_H

#include <string>

class Particle {
public:
    double x, y, z;
    double vx, vy, vz;
    std::string type;
    double distance_in_shield;
    bool alive;

    Particle(double x, double y, double z,
             double vx, double vy, double vz,
             std::string type);

    void move(double dt);
};

#endif
