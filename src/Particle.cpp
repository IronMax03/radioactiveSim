/**
 * @file Particle.cpp
 * @brief This file contains the implementation of the Particle class methods.
 */
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

/// @brief 
/// @param dt 
/// @throws std::runtime_error if dt is negative or zero.
/// @note this error can be ignored whith a try catch if the final user wants to use a negative or zero time step.
/// @note This error is mainly here to prevent the user from accidentally using a negative or zero time step, which would lead to undefined behavior in the simulation.
void Particle::move(double dt) 
{    
    position += velocity * dt;

    if (dt <= 0)
        throw std::runtime_error("Particle movement error: Time step must be positive and not equal to zero.");
}
