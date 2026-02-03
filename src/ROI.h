#ifndef ROI_H
#define ROI_H

#include "Particle.h"

class ROI {
public:
    double xmin, xmax, ymin, ymax, zmin, zmax;
    int count;

    ROI(double xmin, double xmax,
        double ymin, double ymax,
        double zmin, double zmax);

    bool contains(const Particle& p) const;
    void score(Particle& p);
};

#endif
