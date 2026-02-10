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
    // implement the method that checks if a particle is inside the shielding
    // normalized coordinate
    double nx = (p.x - xmin)/(xmax - xmin);
    double ny = (p.y - ymin)/(ymax - ymin);
    double nz = (p.z - zmin)/(zmax - zmin);

    return nx >= 0 && nx <= 1 &&
           ny >= 0 && ny <= 1 &&
           nz >= 0 && nz <= 1;
}
