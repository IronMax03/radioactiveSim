#include "Shielding.h"

Shielding::Shielding(double xmin_, double xmax_,
                     double ymin_, double ymax_,
                     double zmin_, double zmax_,
                     Material m)
    : min_corner(xmin_, ymin_, zmin_),
      max_corner(xmax_, ymax_, zmax_),
      material(m)
{
  if (min_corner.x > max_corner.x || min_corner.y > max_corner.y || min_corner.z > max_corner.z)
    throw std::invalid_argument("Invalid shielding dimensions: min corner must be less than max corner.");
  else if (min_corner == max_corner)
    throw std::invalid_argument("Invalid shielding dimensions: min corner and max corner cannot be the same point.");
}

bool Shielding::contains(const Particle& p) const 
{
    // implement the method that checks if a particle is inside the shielding
    // normalized coordinates
    vector3<double> n = (p.position - min_corner) / (max_corner - min_corner);

    return n.x >= 0 && n.x <= 1 &&
           n.y >= 0 && n.y <= 1 &&
           n.z >= 0 && n.z <= 1;
}
