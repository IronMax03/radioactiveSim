#include "ROI.h"

ROI::ROI(double xmin_, double xmax_,
         double ymin_, double ymax_,
         double zmin_, double zmax_)
    : min_corner(vector3<double>{xmin_, ymin_, zmin_}),
      max_corner(vector3<double>{xmax_, ymax_, zmax_}),
      count(0)
{}

/// @brief Return True if the particle is inside the ROI, False otherwise.
bool ROI::contains(const Particle& p) const {
    //normalized coordinate
    vector3<double> n = (p.position - min_corner) / (max_corner - min_corner).norm();

    return n.x >= 0 && n.x <= 1 &&
           n.y >= 0 && n.y <= 1 &&
           n.z >= 0 && n.z <= 1;
}

/// @brief If the particle is alive and inside the ROI, increment the count and mark the particle as not alive anymore.
void ROI::score(Particle& p) 
{
    if (p.alive && contains(p)) 
    {
        count++;
        // Do not consider particle anymore once we have counted it
        p.alive = false;
    }
}
