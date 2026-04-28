/// @file GeometryOutput.h
/// @brief Declarations for geometry output utilities: writing bounding-box meshes
///        in VTP format and enforcing the simulation domain boundary.

#pragma once

#include <string>
#include "Particle.h"
#include "Box.h"

/// @brief Write an axis-aligned box as a VTK PolyData (@c .vtp) file.
///
/// The box is represented as a mesh of 8 vertices and 6 quad faces.
/// Additionally, the function tracks the global bounding box of all boxes
/// written so far (used by is_in_bounding_box()).
///
/// @param xmin     Minimum x-coordinate of the box (metres).
/// @param xmax     Maximum x-coordinate of the box (metres).
/// @param ymin     Minimum y-coordinate of the box (metres).
/// @param ymax     Maximum y-coordinate of the box (metres).
/// @param zmin     Minimum z-coordinate of the box (metres).
/// @param zmax     Maximum z-coordinate of the box (metres).
/// @param filename Path to the output @c .vtp file.
void write_box_vtp(double xmin, double xmax,
                   double ymin, double ymax,
                   double zmin, double zmax,
                   const std::string& filename);

/// @brief Kill a particle if it has left the global bounding box.
///
/// The global bounding box is the union of all boxes passed to write_box_vtp().
/// If @p p is outside this box its @c alive flag is set to @c false.
///
/// @param p The particle to test and potentially deactivate.
void is_in_bounding_box(Particle& p);
