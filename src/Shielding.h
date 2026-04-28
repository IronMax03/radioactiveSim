#pragma once

#include "Material.h"
#include "Particle.h"
#include "Box.hpp"

class Shielding {
public:
    //double xmin, xmax, ymin, ymax, zmin, zmax;
    vector3<double> min_corner;
    vector3<double> max_corner;
    Material material;

    Shielding(double xmin, double xmax,
              double ymin, double ymax,
              double zmin, double zmax,
              Material m);

    bool contains(const Particle& p) const;
};
