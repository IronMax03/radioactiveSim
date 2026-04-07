#include "Particle.h"

Particle::Particle(double x_, double y_, double z_,
                   double vx_, double vy_, double vz_,
                   std::string t)
    : x(x_), y(y_), z(z_),
      vx(vx_), vy(vy_), vz(vz_),
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
    : x(x_), y(y_), z(z_),
      vx(vx_), vy(vy_), vz(vz_),
      type(t),
      distance_in_shield(0.0),
      electric_charge(ec),
      alive(true)
      {}

void Particle::move(double dt) 
{
    x += vx*dt;
    y += vy*dt;
    z += vz*dt;
}

bool Particle::operator==(const Particle& p) 
{
    return p.x == this->x   && p.y == this->y   && p.z == this->z   &&
           p.vx == this->vx && p.vy == this->vy && p.vz == this->vz &&
           p.type == this->type;
}

