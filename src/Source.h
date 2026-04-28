/// @file Source.h
/// @brief Declaration of the Source class, which models a radioactive point source
///        that emits particles isotropically at a configurable rate.

#pragma once

#include <vector>
#include <string>
#include "Particle.h"

/// @brief Models a radioactive point source that emits particles every time step.
///
/// The source is located at a fixed 3-D position and emits a given number of
/// particles per time step. Each emitted particle has a randomised direction and
/// speed drawn from statistical distributions, and its type (proton or neutron)
/// is chosen randomly with a 3:1 proton-to-neutron ratio.
class Source {
public:
    /// @brief X-coordinate of the source position (metres).
    double x;

    /// @brief Y-coordinate of the source position (metres).
    double y;

    /// @brief Z-coordinate of the source position (metres).
    double z;

    /// @brief Number of particles emitted per time step.
    int rate;

    /// @brief Construct a source at a given position with a given emission rate.
    /// @param x    X-coordinate of the source (metres).
    /// @param y    Y-coordinate of the source (metres).
    /// @param z    Z-coordinate of the source (metres).
    /// @param rate Number of particles to emit per time step. Must be positive.
    Source(double x, double y, double z,
           int rate);

    /// @brief Emit one batch of particles for the current time step.
    ///        The number of particles returned equals @c rate.
    ///        Each particle's direction and speed are sampled from normal distributions,
    ///        and its type is chosen randomly (75 % proton, 25 % neutron).
    /// @return A vector containing the newly emitted Particle objects.
    std::vector<Particle> emit() const;
};
