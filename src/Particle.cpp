/// @file Particle.cpp
/// @brief Implementation of the Particle class methods.

#include "Particle.h"

/// @brief Default constructor. Initialises all kinematic fields to zero and marks the particle as dead.
Particle::Particle()
    : position(vector3<double>{0, 0, 0}),
    velocity(vector3<double>{0, 0, 0}),
    acceleration(vector3<double>{0.0, 0.0, 0.0}),
    mass(0),
    electric_charge(0),
    type(""),
    distance_in_shield(0.0),
    alive(false) {}

/// @brief Construct a particle from position, velocity, and type string.
///        Mass and electric charge are inferred automatically from @p t.
/// @param x_  Initial x-coordinate.
/// @param y_  Initial y-coordinate.
/// @param z_  Initial z-coordinate.
/// @param vx_ Initial x-component of velocity.
/// @param vy_ Initial y-component of velocity.
/// @param vz_ Initial z-component of velocity.
/// @param t   Particle type string. Accepted values: @c "proton", @c "neutron".
/// @throws std::runtime_error if @p t is not a recognised particle type.
Particle::Particle(double x_, double y_, double z_,
                   double vx_, double vy_, double vz_,
                   std::string t)
    : position(vector3<double>{x_,y_,z_}),
      velocity(vector3<double>{vx_,vy_,vz_}),
      acceleration(vector3<double>{0.0, 0.0, 0.0}),
      type(t),
      distance_in_shield(0.0),
      alive(true)
{
    if (t == "proton")
    {
        electric_charge = 1;
        mass = 1;
    }
    else if (t == "neutron")
    {
        electric_charge = 0;
        mass = 1;
    }
    else
        throw std::runtime_error("Particle constructor error: Invalid particle type. Valid types are: 'proton', 'neutron', and 'electron'.");
}

/// @brief Construct a particle with explicitly provided mass, charge, and type.
/// @param x_  Initial x-coordinate.
/// @param y_  Initial y-coordinate.
/// @param z_  Initial z-coordinate.
/// @param vx_ Initial x-component of velocity.
/// @param vy_ Initial y-component of velocity.
/// @param vz_ Initial z-component of velocity.
/// @param m   Mass in units of proton mass.
/// @param ec  Electric charge in coulomb units.
/// @param t   Particle type label.
Particle::Particle(double x_, double y_, double z_,
                   double vx_, double vy_, double vz_,
                    double m,
                   int ec,
                   std::string t)
    : position(vector3<double>{x_,y_,z_}),
      velocity(vector3<double>{vx_,vy_,vz_}),
      acceleration(vector3<double>{0.0, 0.0, 0.0}),
      mass(m),
      electric_charge(ec),
      type(t),
      distance_in_shield(0.0),
      alive(true)
      {}

/// @brief Move the particle by updating its position and velocity based on its acceleration and the time step dt.
/// @param dt The time step to move the particle. Must be positive and not equal to zero.
/// @throws std::runtime_error if dt is negative or zero.
/// @note this error can be ignored whith a try catch if the final user wants to use a negative or zero time step.
/// @note This error is mainly here to prevent the user from accidentally using a negative or zero time step, which would lead to undefined behavior in the simulation.
void Particle::move(double dt) 
{
    velocity += acceleration * dt;
    position += velocity * dt;

    if (dt <= 0)
        throw std::runtime_error("Particle movement error: Time step must be positive and not equal to zero.");
}
