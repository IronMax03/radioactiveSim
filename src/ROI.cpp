#include "ROI.h"

ROI::ROI(double xmin_, double xmax_,
         double ymin_, double ymax_,
         double zmin_, double zmax_)
    : xmin(xmin_), xmax(xmax_),
      ymin(ymin_), ymax(ymax_),
      zmin(zmin_), zmax(zmax_),
      count(0)
{}

bool ROI::contains(const Particle& p) const {
    // TODO implement the method that checks if a particle is inside the ROI
    return false; // TODO this is a placeholder return value
}

void ROI::score(Particle& p) {
    if (p.alive && contains(p)) {
        count++;
        // Do not consider particle anymore once we have counted it
        p.alive = false;
    }
}
