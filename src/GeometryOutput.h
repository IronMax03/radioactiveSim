#pragma once

#include <string>
#include "Particle.h"
#include "Box.hpp"

void write_box_vtp(double xmin, double xmax,
                   double ymin, double ymax,
                   double zmin, double zmax,
                   const std::string& filename);

void is_in_bounding_box(Particle& p);
