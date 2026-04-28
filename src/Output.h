/// @file Output.h
/// @brief Declarations for CSV output utilities used to persist particle state.

#pragma once

#include <vector>
#include <string>
#include "Particle.h"

/// @brief Write the state of a collection of particles to a CSV file.
///
/// The file will have a header row followed by one data row per particle.
/// Columns: @c x, @c y, @c z, @c alive, @c type.
///
/// @param particles The collection of particles to serialise.
/// @param filename  Path to the output CSV file. The file is created or overwritten.
/// @throws If the file cannot be opened, an error message is printed and an
///         exception is re-thrown.
void write_particles_csv(const std::vector<Particle>& particles,
                          const std::string& filename);
