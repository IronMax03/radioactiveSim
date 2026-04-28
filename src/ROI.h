/// @file ROI.h
/// @brief Declaration of the ROI (Region Of Interest) class, which defines an
///        axis-aligned box used to score particles that pass through it.

#pragma once

#include "Particle.h"

/// @brief An axis-aligned bounding box used to score particles entering the region.
///
/// The ROI counts each live Particle that enters its volume and marks that
/// particle as no longer alive so it is not counted more than once.
class ROI {
public:
    /// @brief Minimum corner of the axis-aligned bounding box (metres).
    vector3<double> min_corner;

    /// @brief Maximum corner of the axis-aligned bounding box (metres).
    vector3<double> max_corner;

    /// @brief Cumulative count of particles scored inside this ROI.
    int count;

    /// @brief Construct a ROI from explicit axis-aligned bounds.
    /// @param xmin Minimum x-coordinate of the ROI (metres).
    /// @param xmax Maximum x-coordinate of the ROI (metres).
    /// @param ymin Minimum y-coordinate of the ROI (metres).
    /// @param ymax Maximum y-coordinate of the ROI (metres).
    /// @param zmin Minimum z-coordinate of the ROI (metres).
    /// @param zmax Maximum z-coordinate of the ROI (metres).
    ROI(double xmin, double xmax,
        double ymin, double ymax,
        double zmin, double zmax);

    /// @brief Test whether a particle is currently inside the ROI.
    /// @param p The particle to test.
    /// @return @c true if @p p is within the axis-aligned bounding box, @c false otherwise.
    bool contains(const Particle& p) const;

    /// @brief Score a particle if it is alive and inside the ROI.
    ///        Increments @c count and marks the particle as dead to prevent double-counting.
    /// @param p The particle to evaluate. Modified in-place if scored.
    void score(Particle& p);
};
