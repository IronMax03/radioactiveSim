#include "Shielding.h"

Shielding::Shielding(double xmin_, double xmax_,
                     double ymin_, double ymax_,
                     double zmin_, double zmax_,
                     Material m)
    : xmin(xmin_), xmax(xmax_),
      ymin(ymin_), ymax(ymax_),
      zmin(zmin_), zmax(zmax_),
      material(m)
{}

bool Shielding::contains(const Particle& p) const {
    // TODO implement the method that checks if a particle is inside the shielding
    return false; // TODO this is a placeholder return value
}
