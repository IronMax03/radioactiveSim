/// @file Material.h
/// @brief Declaration of the Material struct, which describes the physical
///        properties of a shielding material relevant to particle attenuation.

#pragma once

#include <string>

/// @brief Describes the physical properties of a shielding material.
///
/// Currently the only property used by the simulation is the mean free path,
/// which governs exponential attenuation of particles passing through the material.
struct Material {
    /// @brief Human-readable name of the material (e.g. @c "Lead", @c "Vacuum").
    std::string name;

    /// @brief Mean free path of the material in metres.
    ///        Larger values mean the material is more transparent to radiation.
    ///        Defaults to @c 1e9 (effectively transparent, i.e. vacuum).
    double mean_free_path;

    /// @brief Construct a material with a given name and mean free path.
    /// @param n   Name of the material. Defaults to @c "Vacuum".
    /// @param mfp Mean free path in metres. Defaults to @c 1e9 (vacuum).
    Material(std::string n = "Vacuum", double mfp = 1e9)
        : name(n), mean_free_path(mfp) {}
};
