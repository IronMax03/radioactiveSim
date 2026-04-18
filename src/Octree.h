/**
 * @file Octree.h
 * @brief This file contains the definition of the Octree class, which is a spatial partition.
 */
#pragma once

#include "Particle.h"

#include <memory>
#include <type_traits>


/// @brief A lazy node for the octree data structure.
/// @tparam T The type of the coordinates. Must be a numeric type (int, float, double, etc.).
/// @note Spatial indexing is done using morton indices, which means that the children of a node are indexed as follows:
///          0: lower-left-back
///          1: lower-right-back
///          2: upper-left-back
///          3: upper-right-back
///          4: lower-left-front
///          5: lower-right-front
///          6: upper-left-front
///          7: upper-right-front
template<typename T>
struct lazy_node
{
    static_assert(std::is_integral_v<T> || std::is_floating_point_v<T>);
      
    vector3<T> box_center;
    vector3<T> center_of_charge;

    T box_length;
    int total_charge;

    std::unique_ptr<lazy_node<T>> childs[8];
    std::shared_ptr<Particle> particle;

    /// @brief Return True if nth child is evaluated (exist).
    constexpr bool is_child_inst(int n) const { return childs[n] != nullptr; }

    constexpr double Calc_BH_criteria(vector3<T> position) const { return box_length / (center_of_charge - position).norm(); }

    /// @brief Return True if the barnes hut criteria is satisfied current node.
    /// @param x X coordinate of the position of the particle we want to calculate the force from this node on.
    /// @param y Y coordinate of the position of the particle we want to calculate the force from this node on.
    /// @param z Z coordinate of the position of the particle we want to calculate the force from this node on.
    /// @param theta Theta parameter for the barnes hut criteria.
    constexpr bool BH_criteria(const T& x, const T& y, const T& z, float theta) const { return Calc_BH_criteria(vector3<T>{x,y,z}) < theta; }

    /// @brief Return True if the barnes hut criteria is satisfied current node.
    /// @param position Position of the particle we want to calculate the force from this node on.
    /// @param theta Theta parameter for the barnes hut criteria.
    constexpr bool BH_criteria(vector3<T> position, float theta) const { return Calc_BH_criteria(position) < theta; }

    /// @brief Calculate the center of mass and total charge
    inline void eval()
    { 
        center_of_charge = particle->position;
        total_charge = particle->electric_charge;

        for (std::unique_ptr<lazy_node<T>>& c: childs)
        {
            if (c != nullptr)
            {
                c->eval();
                total_charge += c->total_charge;
                center_of_charge = (center_of_charge + c->center_of_charge)/2;
            }
        }
    }

    /// @brief Calculate the center of mass and total charge of the n-th child
    inline void eval(const size_t& n) { get(n).eval(); }

    inline void instantiate_child(const size_t& n)
    {
        if (n > 7)
            throw  std::runtime_error("lazy_node.instantiate_child(n): n must be smaller or equal to 7.");

        childs[n] = std::make_unique<lazy_node<T>>(); 
    }

    /// @brief Return the nth child.
    /// @param n The index of the child to return. The indices are defined by the morton indices.
    inline lazy_node& get(const size_t& n) 
    {
        if (n > 7)
            throw  std::runtime_error("lazy_node.get(n): n must be smaller or equal to 7.");
        else if (childs[n] == nullptr)
            instantiate_child(n);
        return *childs[n];
    }
};


/// @brief A spatial partitioning octree data structure for implementing the Barnes-Hut algorithm.
class Octree
{
    private:
        /// @brief The type of the coordinates of the octree. Must be a numeric type (int, float, double, etc.).
        typedef int nt;

        lazy_node<nt> root;

        size_t convert_to_morton_code(const vector3<nt>& position) const;
        lazy_node<nt> build(const lazy_node<nt>& node, const Particle& particles);

    public:
        Octree(nt bound_xmin, nt bound_xmax, nt bound_ymin, nt bound_ymax, nt bound_zmin, nt bound_zmax);

        lazy_node<nt>& get_root();

        void set_default_field(Octree def);
        void add_point(const Particle& p);

};
