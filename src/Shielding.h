/// @file Shielding.h
/// @brief Declaration of the Shielding class, which models an axis-aligned
///        rectangular shielding volume filled with a given material.

#pragma once

#include "Material.h"
#include "Particle.h"
#include "Box.hpp"

/// @brief An axis-aligned rectangular volume of shielding material.
///
/// The Shielding class defines a box-shaped region of space that attenuates
/// particles passing through it according to the mean free path of its material.
class Shielding {
public:
    /// @brief Minimum corner of the axis-aligned bounding box (metres).
    vector3<double> min_corner;

    /// @brief Maximum corner of the axis-aligned bounding box (metres).
    vector3<double> max_corner;

    /// @brief Physical material that fills this shielding volume.
    Material material;

    /// @brief Construct a shielding volume from explicit axis-aligned bounds and a material.
    /// @param xmin Minimum x-coordinate of the shielding box (metres).
    /// @param xmax Maximum x-coordinate of the shielding box (metres).
    /// @param ymin Minimum y-coordinate of the shielding box (metres).
    /// @param ymax Maximum y-coordinate of the shielding box (metres).
    /// @param zmin Minimum z-coordinate of the shielding box (metres).
    /// @param zmax Maximum z-coordinate of the shielding box (metres).
    /// @param m    Material that fills the shielding volume.
    /// @throws std::invalid_argument if any min coordinate is greater than the corresponding max,
    ///         or if min and max corners coincide (zero-volume box).
    Shielding(double xmin, double xmax,
              double ymin, double ymax,
              double zmin, double zmax,
              Material m);

    /// @brief Test whether a particle is currently inside the shielding volume.
    /// @param p The particle to test.
    /// @return @c true if @p p is within the shielding box, @c false otherwise.
    bool contains(const Particle& p) const;
};
