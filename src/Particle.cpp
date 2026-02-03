#include "Particle.h"

Particle::Particle(double x_, double y_, double z_,
                   double vx_, double vy_, double vz_,
                   std::string t)
    : x(x_), y(y_), z(z_),
      vx(vx_), vy(vy_), vz(vz_),
      type(t),
      distance_in_shield(0.0),
      alive(true)
{}

// TODO test function
void Particle::move(double dt) {
    // this is done by just updating the position based on velocity and time step
    x += vx*dt;
    y += vy*dt;
    z += vz*dt;
}
