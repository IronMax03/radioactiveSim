#ifndef SHIELDING_H
#define SHIELDING_H

#include "Material.h"
#include "Particle.h"

class Shielding {
public:
    double xmin, xmax, ymin, ymax, zmin, zmax;
    Material material;

    Shielding(double xmin, double xmax,
              double ymin, double ymax,
              double zmin, double zmax,
              Material m);

    bool contains(const Particle& p) const;
};

#endif
