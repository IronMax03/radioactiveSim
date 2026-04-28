/// @file Shielding.cpp
/// @brief Implementation of the Shielding class methods.

#include "Shielding.h"

/// @brief Construct a shielding volume from explicit axis-aligned bounds and a material.
/// @param xmin_ Minimum x-coordinate (metres).
/// @param xmax_ Maximum x-coordinate (metres).
/// @param ymin_ Minimum y-coordinate (metres).
/// @param ymax_ Maximum y-coordinate (metres).
/// @param zmin_ Minimum z-coordinate (metres).
/// @param zmax_ Maximum z-coordinate (metres).
/// @param m     Material that fills the volume.
/// @throws std::invalid_argument if any min coordinate exceeds the corresponding max,
///         or if the min and max corners are identical (zero-volume box).
Shielding::Shielding(double xmin_, double xmax_,
                     double ymin_, double ymax_,
                     double zmin_, double zmax_,
                     Material m)
    : min_corner(vector3<double>{xmin_, ymin_, zmin_}),
      max_corner(vector3<double>{xmax_, ymax_, zmax_}),
      material(m)
{
  if (min_corner.x > max_corner.x || min_corner.y > max_corner.y || min_corner.z > max_corner.z)
    throw std::invalid_argument("Invalid shielding dimensions: min corner must be less than max corner.");
  else if (min_corner == max_corner)
    throw std::invalid_argument("Invalid shielding dimensions: min corner and max corner cannot be the same point.");
}

/// @brief Test whether a particle is currently inside the shielding volume.
///        Uses normalised coordinates relative to the bounding box extents.
/// @param p The particle to test.
/// @return @c true if all normalised coordinates of @p p are in [0, 1], @c false otherwise.
bool Shielding::contains(const Particle& p) const 
{
    // implement the method that checks if a particle is inside the shielding
    // normalized coordinates
    vector3<double> n = (p.position - min_corner) / (max_corner - min_corner).norm();

    return n.x >= 0 && n.x <= 1 &&
           n.y >= 0 && n.y <= 1 &&
           n.z >= 0 && n.z <= 1;
}
