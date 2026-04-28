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
void Octree::add_particle(const Particle& p, lazy_node<nt>& node, int depth)
{
    // Guard against infinite recursion from coincident (same-position) particles.
    if (depth > max_depth)
        return;

    if (node.is_leaf() && node.particle == nullptr)
    {
        node.particle = std::make_shared<Particle>(p);
    }
    else
    {
        add_particle(p,node.get(node.child_index(p.position)), depth + 1);

        if (node.particle != nullptr)
        {
            // Copy and clear before recursing — the old direct assignment
            // silently dropped a particle when two mapped to the same child.
            Particle existing = *(node.particle);
            node.particle = nullptr;
            add_particle(existing, node.get(node.child_index(existing.position)), depth + 1);
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

/// @brief Effective Coulomb constant for the simulation unit system.
///        Units: charge in elementary charges (e), mass in proton masses (m_p), length in metres.
///        k_eff = k_e · e² / m_p ≈ 0.1379 m³·m_p⁻¹·s⁻²
///        This gives accelerations directly in m/s² when dividing by mass in proton-mass units.
static constexpr double k_e = 0.13794;

/// @brief Softening length squared (m²) to cap forces at very short range.
static constexpr double eps2 = 1e-4;

/// @brief Recursive Barnes-Hut traversal.  Accumulates the Coulomb force on
///        the target particle @p p from the charges stored in @p node.
///
///        At each internal node the opening-angle criterion s/d < θ is checked.
///        If satisfied the node's aggregate charge & centre-of-charge are used
///        directly (O(1)); otherwise the traversal recurses into the children.
///
/// @param p     Target particle (read-only – force is accumulated in @p force).
/// @param node  Current node being considered.
/// @param force Running force accumulator.
void Octree::calc_force_recursive(const Particle& p, lazy_node<nt>& node,
                                  vector3<double>& force)
{
    if (node.is_leaf())
    {
        if (node.particle != nullptr)
        {
            // Skip self-interaction.
            if (node.particle->position.x == p.position.x &&
                node.particle->position.y == p.position.y &&
                node.particle->position.z == p.position.z)
                return;

            vector3<double> r {
                p.position.x - node.particle->position.x,
                p.position.y - node.particle->position.y,
                p.position.z - node.particle->position.z
            };
            double dist2 = r.x*r.x + r.y*r.y + r.z*r.z;
            if (dist2 < eps2) return;

            double dist  = std::sqrt(dist2);
            // F = k_e * q_target * q_source / |r|^2  in the direction of r_hat
            // (like charges ⇒ repulsive ⇒ force along r  = target − source)
            double coeff = k_e * p.electric_charge * node.particle->electric_charge / (dist2 * dist);
            force.x += coeff * r.x;
            force.y += coeff * r.y;
            force.z += coeff * r.z;
        }
        return;
    }

    if (node.BH_criteria(p.position, theta))
    {
        node.eval();

        vector3<double> r {
            p.position.x - node.center_of_charge.x,
            p.position.y - node.center_of_charge.y,
            p.position.z - node.center_of_charge.z
        };
        double dist2 = r.x*r.x + r.y*r.y + r.z*r.z;
        if (dist2 < eps2) return;

        double dist  = std::sqrt(dist2);
        double coeff = k_e * p.electric_charge * node.total_charge
                       / (dist2 * dist);
        force.x += coeff * r.x;
        force.y += coeff * r.y;
        force.z += coeff * r.z;
        return;
    }

    for (size_t i = 0; i < 8; ++i)
    {
        if (node.is_child_inst(i))
            calc_force_recursive(p, *node.childs[i], force);
    }
}

/// @brief Compute the net electrostatic force on @p p via the Barnes-Hut approximation
///        and update @p p.acceleration accordingly.
///
///        The tree must already be populated (via build() / add_particle()).
///        This function first calls eval() on the root to (re-)compute aggregate
///        charges and centres-of-charge, then performs the recursive traversal.
///
/// @param p The particle for which to compute the acceleration. Modified in-place.
void Octree::calc_force(Particle& p)
{
    vector3<double> force{0.0, 0.0, 0.0};

    calc_force_recursive(p, root, force);

    p.acceleration = force;
}