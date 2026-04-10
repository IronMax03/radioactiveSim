#include "Particle.h"

Particle::Particle(double x_, double y_, double z_,
                   double vx_, double vy_, double vz_,
                   std::string t)
    : position(vector3<double>{x_,y_,z_}),
      velocity(vector3<double>{vx_,vy_,vz_}),
      type(t),
      distance_in_shield(0.0),
      alive(true)
{
    if (t == "proton")
        electric_charge = 1;
    else if (t == "neutron")
        electric_charge = 0;
    else if (t == "electron")
        electric_charge = -1;
}

Particle::Particle(double x_, double y_, double z_,
                   double vx_, double vy_, double vz_,
                   int ec,
                   std::string t)
    : position(vector3<double>{x_,y_,z_}),
      velocity(vector3<double>{vx_,vy_,vz_}),
      electric_charge(ec),
      type(t),
      distance_in_shield(0.0),
      alive(true)
      {}

void Particle::move(double dt) 
{
    position += velocity * dt;
}
