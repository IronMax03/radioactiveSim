#include "ROI.h"

ROI::ROI(double xmin_, double xmax_,
         double ymin_, double ymax_,
         double zmin_, double zmax_)
    : xmin(xmin_), xmax(xmax_),
      ymin(ymin_), ymax(ymax_),
      zmin(zmin_), zmax(zmax_),
      count(0)
{}

inline bool ROI::contains(const Particle& p) const {
    //normalized coordinate
    double nx = (p.x - xmin)/(xmax - xmin);
    double ny = (p.y - ymin)/(ymax - ymin);
    double nz = (p.z - zmin)/(zmax - zmin);

    return nx >= 0 && nx <= 1 &&
           ny >= 0 && ny <= 1 &&
           nz >= 0 && nz <= 1;
}

void ROI::score(Particle& p) {
    if (p.alive && contains(p)) {
        count++;
        // Do not consider particle anymore once we have counted it
        p.alive = false;
    }
}
