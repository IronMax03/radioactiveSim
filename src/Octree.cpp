/// @file Octree.cpp
/// @brief This file contains the implementation of the Octree class and its member functions.
/// @author Maximilien Notz

#include "Octree.h"
#include <cmath>
#include <cstddef>
#include <cstdlib>

/// @brief Construct an Octree that covers the axis-aligned box [minVec, maxVec].
///        The root bounding box becomes the smallest cube enclosing that box.
/// @param minVec Minimum corner of the region to partition.
/// @param maxVec Maximum corner of the region to partition.
Octree::Octree(vector3<nt> minVec, vector3<nt> maxVec)
{
    root.bounding_box.center = (maxVec + minVec)/2;

    vector3<nt> temp = maxVec - minVec;
    root.bounding_box.length = std::abs(temp.x);

    if (root.bounding_box.length < std::abs(temp.y))
        root.bounding_box.length = std::abs(temp.y);
    if (root.bounding_box.length < std::abs(temp.z))
        root.bounding_box.length = std::abs(temp.z);
}

/// @brief return the root node of the octree.
/// @note This function is intended for future scaling purposes, the current implementation is intrusive for testing purposes and should be replaced with a encapsulated version.
lazy_node<nt>& Octree::get_root() { return this->root; }

/// @brief This is a private helper function for the public add_particle function. It adds a particle to the octree by recursively traversing the tree and adding the particle to the appropriate node.
/// @param p The particle to add to the octree.
/// @param node The root node of the octree or subtree to add the particle to.
void Octree::add_particle(const Particle& p, lazy_node<nt>& node)
{
    if (node.is_leaf() && node.particle == nullptr)
    {
        node.particle = std::make_shared<Particle>(p);
    }
    else
    {
        add_particle(p,node.get(node.child_index(p.position)));

        if (node.particle != nullptr)
        {
            node.get(node.child_index(node.particle->position)).particle = node.particle;
            node.particle = nullptr;
        }
    }
}

/// @brief Add a particle to the octree. This is a public wrapper function for the private add_particle function that takes a node as an argument.
/// @param p The particle to add to the octree.
void Octree::add_particle(const Particle& p) { add_particle(p, this->root); }

/// @brief Build the octree from a vector of particles.
/// @param particles The vector of particles to build the octree from.
void Octree::build(const std::vector<Particle>& particles)
{
    for (const Particle& p : particles)
        add_particle(p, this->root);
}

/// @brief Compute the net electrostatic force on @p p via the Barnes-Hut approximation
///        and update @p p.acceleration accordingly.
/// @note The actual Barnes-Hut traversal is not yet implemented; the acceleration is
///       currently set to zero as a placeholder.
/// @param p The particle for which to compute the acceleration. Modified in-place.
void Octree::calc_force(Particle& p)
{
    vector3<double> force{0.0, 0.0, 0.0};



    p.acceleration = force;
}
