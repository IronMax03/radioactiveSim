#include "Octree.h"
#include <cmath>
#include <cstdlib>

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

const lazy_node<nt>& Octree::get_root() const { return root; }

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
            node.get(node.child_index(p.position)).particle = node.particle;
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
    for (const auto& p : particles)
        add_particle(p, this->root);
}
