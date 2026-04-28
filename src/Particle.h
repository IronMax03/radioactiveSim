/// @file Particle.h
/// @brief Declaration of the Particle class, representing a physical particle
///        (e.g. proton or neutron) tracked throughout the simulation.

#pragma once

#include "utils.h"

#include <string>

/// @brief Represents a single physical particle moving through the simulation domain.
///
/// A Particle carries kinematic state (position, velocity, acceleration), physical
/// properties (mass, electric charge, type), and bookkeeping information used by the
/// simulation loop (distance travelled inside a shield, alive flag).
class Particle {
public:
    /// @brief Current 3-D position of the particle (metres).
    vector3<double> position;

    /// @brief Current 3-D velocity of the particle (metres per second).
    vector3<double> velocity;

    /// @brief Current 3-D acceleration of the particle (metres per second squared).
    vector3<double> acceleration;

    /// @brief Mass of the particle expressed in units of proton mass.
    double mass;

    /// @brief Electric charge of the particle in coulomb units (integer).
    int electric_charge;

    /// @brief Particle species identifier (e.g. @c "proton" or @c "neutron").
    std::string type;

    /// @brief Cumulative distance the particle has travelled inside a shielding volume (metres).
    double distance_in_shield;

    /// @brief Whether the particle is still active in the simulation.
    ///        Set to @c false once the particle is absorbed or leaves the domain.
    bool alive;

    /// @brief Default constructor. Creates a stationary, uncharged, dead particle at the origin.
    Particle();

    /// @brief Construct a particle with position, velocity, and type.
    ///        Mass and electric charge are inferred from @p type.
    /// @param x  Initial x-coordinate (metres).
    /// @param y  Initial y-coordinate (metres).
    /// @param z  Initial z-coordinate (metres).
    /// @param vx Initial x-component of velocity (m/s).
    /// @param vy Initial y-component of velocity (m/s).
    /// @param vz Initial z-component of velocity (m/s).
    /// @param type Particle species. Accepted values: @c "proton", @c "neutron".
    /// @throws std::runtime_error if @p type is not a recognised particle species.
    Particle(double x, double y, double z,
             double vx, double vy, double vz,
             std::string type);

    /// @brief Construct a particle with position, velocity, explicit mass, charge, and type.
    /// @param x  Initial x-coordinate (metres).
    /// @param y  Initial y-coordinate (metres).
    /// @param z  Initial z-coordinate (metres).
    /// @param vx Initial x-component of velocity (m/s).
    /// @param vy Initial y-component of velocity (m/s).
    /// @param vz Initial z-component of velocity (m/s).
    /// @param m  Mass in units of proton mass.
    /// @param ec Electric charge in coulomb units.
    /// @param type Particle species label.
    Particle(double x, double y, double z,
             double vx, double vy, double vz,
             double m,
             int ec,
             std::string type);

    /// @brief Advance the particle by one time step @p dt.
    ///        Updates velocity from acceleration, then position from velocity.
    /// @param dt Time step duration (seconds). Must be strictly positive.
    /// @throws std::runtime_error if @p dt is less than or equal to zero.
    void move(double dt);
};
