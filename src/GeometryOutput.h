#ifndef GEOMETRY_OUTPUT_H
#define GEOMETRY_OUTPUT_H

#include <string>

void write_box_vtp(double xmin, double xmax,
                   double ymin, double ymax,
                   double zmin, double zmax,
                   const std::string& filename);

#endif
