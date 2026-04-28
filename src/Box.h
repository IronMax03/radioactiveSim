/// @file Box.hpp
/// @brief Defines the Box class representing a 3D box in space, used for shielding and geometry output.
/// @author Siddesh Pujari

#pragma once

#include "Particle.h"
#include "utils.h"

class Box 
{
    private:
        vector3<double> min, max;

    public:

        Box(double x1, double x2, double y1, double y2, double z1, double z2)
        {
            if (x1 < x2)
            {
                min.x = x1;
                max.x = x2;
            }
            else
            {
                min.x = x2;
                max.x = x1;
            }

            if (y1 < y2)
            {
                min.y = y1;
                max.y = y2;
            }
            else
            {
                min.y = y2;
                max.y = y1;
            }

            if (z1 < z2)
            {
                min.z = z1;
                max.z = z2;
            }
            else 
            {
                min.z = z2;
                max.z = z1;
            }
        }

        Box(vector3<double> minVec, vector3<double> maxVec)
            : min(minVec), max(maxVec) {}

        bool contains(const Particle& p) const 
        {
            // implement the method that checks if a particle is inside the shielding
            // normalized coordinates
            vector3<double> n = (p.position - min) / (max - min).norm();

            return n.x >= 0 && n.x <= 1 &&
                n.y >= 0 && n.y <= 1 &&
                n.z >= 0 && n.z <= 1;
        }
};