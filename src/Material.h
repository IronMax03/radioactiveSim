#pragma once

#include <string>

struct Material {
    std::string name;
    double mean_free_path; // meters

    Material(std::string n = "Vacuum", double mfp = 1e9)
        : name(n), mean_free_path(mfp) {}
};
