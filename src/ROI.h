#ifndef ROI_H
#define ROI_H

#include "Particle.h"

class ROI {
public:
    vector3<double> min_corner;
    vector3<double> max_corner;
    int count;

    ROI(double xmin, double xmax,
        double ymin, double ymax,
        double zmin, double zmax);

    bool contains(const Particle& p) const;
    void score(Particle& p);
};

#endif
