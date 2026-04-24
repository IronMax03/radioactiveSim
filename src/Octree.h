/**
 * @file Octree.h
 * @brief This file contains the definition of the Octree class, which is a spatial partition.
 */
#pragma once

#include "Particle.h"

#include <cstddef>
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
    
    struct 
    {
        T length;
        vector3<T> center;
    } bounding_box;

    vector3<T> center_of_charge;
    int total_charge; // The unit is coulombs

    std::unique_ptr<lazy_node<T>> childs[8];
    std::shared_ptr<Particle> particle;

    /// @param p The position of the particle.
    /// @return The child index of the particle position using morton indices.
    /// @throws std::runtime_error if the particle position is not within the bounding box of the node.
    constexpr size_t child_index(const vector3<T>& p) const 
    {
        if (p.x < bounding_box.center.x - bounding_box.length/2 || 
            p.y < bounding_box.center.y - bounding_box.length/2 || 
            p.z < bounding_box.center.z - bounding_box.length/2 ||
            p.x > bounding_box.center.x + bounding_box.length/2 ||
            p.y > bounding_box.center.y + bounding_box.length/2 ||
            p.z > bounding_box.center.z + bounding_box.length/2)
            throw std::runtime_error("lazy_node.child_index(p): Particle position must be within the bounding box of the node.");

        size_t child_index = 0;
        
        // Determine the child index of the particle based on morton indices.
        if (p.x >= bounding_box.center.x)
            child_index = child_index | 0b001; // equivalent to child_index += 1;
        if (p.y >= bounding_box.center.y)
            child_index = child_index | 0b010;// equivalent to child_index += 2;
        if (p.z >= bounding_box.center.z)
            child_index = child_index | 0b100;// equivalent to child_index += 4;

        return child_index;
    }

    /// @brief Return True if nth child is evaluated (exist).
    constexpr bool is_child_inst(const size_t& n) const { return childs[n] != nullptr; }

    /// @brief Return True if the node is a leaf (has no children).
    constexpr bool is_leaf() const 
    { 
        for (const auto& c : childs) 
        {
            if (c != nullptr)
                return false;
        }
        return true;
    }

    /// @brief Calculate the barnes hut criteria for the current node and a given position.
    /// @param position Position of the particle we want to calculate the force from this node on.
    /// @return The barnes hut criteria for the current node and the given position.
    constexpr double Calc_BH_criteria(vector3<T> position) const { return bounding_box.length / (center_of_charge - position).norm(); }

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
        // recursion base case.
        if (is_leaf())
        {
            if (particle != nullptr)
            {
                center_of_charge = particle->position;
                total_charge = particle->electric_charge;
            }


            return;
        }
            
        center_of_charge = vector3<T>{0, 0, 0};
        total_charge = 0;

        for (std::unique_ptr<lazy_node<T>>& c: childs)
        {

            if (c != nullptr)
            {
                if (is_leaf() && c->particle != nullptr)
                    continue; // if the current node is a leaf and the child has a particle, we can skip the evaluation of the child since we will use the particle of the child to calculate the center of charge and total charge of the current node.;

                c->eval();
                center_of_charge = (total_charge*center_of_charge + c->total_charge*c->center_of_charge)/(total_charge + c->total_charge);
                total_charge += c->total_charge;
            }
        }
    }

    /// @brief Calculate the center of mass and total charge of the n-th child
    /// @param n The index of the child to evaluate. The indices are defined by the morton indices.
    inline void eval(const size_t& n) { get(n).eval(); }

    /// @brief Instantiate the nth child if it is not already instantiated.
    /// @param n The index of the child to instantiate. The indices are defined by the morton indices.
    inline void instantiate_child(const size_t& n)
    {
        if (n > 7)
            throw  std::runtime_error("lazy_node.instantiate_child(n): n must be smaller or equal to 7.");

        childs[n] = std::make_unique<lazy_node<T>>(); 
        childs[n]->bounding_box.length = bounding_box.length / 2;
        childs[n]->bounding_box.center = bounding_box.center;
        childs[n]->bounding_box.center.x += (n & 1) ? bounding_box.length/4 : -bounding_box.length/4;
        childs[n]->bounding_box.center.y += (n & 2) ? bounding_box.length/4 : -bounding_box.length/4;
        childs[n]->bounding_box.center.z += (n & 4) ? bounding_box.length/4 : -bounding_box.length/4;
    }

    /// @brief Instantiate the nth child and set its particle to the given particle.
    /// @param n The index of the child to instantiate. The indices are defined by the morton indices.
    /// @param p The particle to set the nth child to.
    inline void create_child(const size_t& n, const Particle& p)
    {
        instantiate_child(n);
        childs[n]->particle = std::make_shared<Particle>(p);
    }


    /// @brief Return the nth child.
    /// @param n The index of the child to return. The indices are defined by the morton indices.
    inline lazy_node& get(const size_t& n) 
    {
        if (n > 7)
            throw  std::runtime_error("lazy_node.get(n): n must be smaller or equal to 7.");
        else if (childs[n] == nullptr)
            instantiate_child(n); // create a dummy particle to instantiate the child if it is not already instantiated.
        return *childs[n];
    }
};

/// @brief The type of the coordinates of the octree. Must be a numeric type (int, float, double, etc.).
typedef double nt;

/// @brief A spatial partitioning octree data structure for implementing the Barnes-Hut algorithm.
class Octree
{
    private:
        lazy_node<nt> root;

        void add_particle(const Particle& p, lazy_node<nt>& node);

    public:
        Octree(vector3<nt> minVec, vector3<nt> maxVec);

        lazy_node<nt>& get_root();
        double calc_force(const Particle& p) const;

        void set_default_field(Octree def);
        void add_particle(const Particle& p);
        void build(const std::vector<Particle>& particles);
};
