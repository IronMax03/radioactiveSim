#ifndef UTILS_H
#define UTILS_H

#include <random>

inline double uniform01() {
    static std::mt19937 gen(42);
    static std::uniform_real_distribution<double> dist(0.0, 1.0);
    return dist(gen);
}

#endif
