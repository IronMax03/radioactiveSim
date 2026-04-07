#pragma once
#include <memory>
#include <type_traits>

template<typename T>
struct lazy_node
{
    static_assert(std::is_integral_v(T) || std::is_floating_point_v(T))

    /// @brief A 3D vector. The type of the components of this vector is defined by T wich can be: int, long, short, double, float, etc.
    struct vector3 { T x,y,z; };

    /// @brief This union is intended to reuse the memory allocated for octree construction (boundaries), during N-Body forces estimation (centerOfmass).
    /// @note In the implementation boundaries is used during the tree construction and centerOfmass is used during the estimate N-Body forces.
    union nodeValue
    {        
        struct boundaries
        { 
            vector3 center;
            T length; 
        } B;

        struct centerOfmass 
        { 
            vector3 cordinate; 
            double mass;
        } CM;
    } value;

    std::unique_ptr<lazy_node<T>> childs[8];

    /// @brief Insures the variable is accessed without been modified.
    inline T read() const { return value; }

    /// @brief Return True if nth child is evaluated (exist).
    inline bool is_child_eval(int n) const { return childs[n] != nullptr; }

    /// @brief Calculate the center of mass and stores it in value.CM
    /// @warning This function destroy any value inside value.B
    inline void eval() 
    { 
        for (auto& std::unique_ptr<lazy_node<T>>: c)
        {
            if (c != nullptr)
                c.eval();
        }
    }

    /// @brief evaluate the child at index n.
    inline void eval(const size_t& n) 
    { 
        childs[n] = std::make_unique<lazy_node<T>>(); 
        childs[n]->eval();
    }

    /// @brief Return the nth child.
    inline lazy_node& get(unsigned int id) 
    {
        if (id > 15)
            throw  std::out_of_range::out_of_range("lazy_node.get(id): id is bigger then 15.");
        else if (childs[id] == nullptr)
            eval(id);
        return *childs[id];
    }
};


// electro dynamic Octree
class Octree
{
    private:
        lazy_node<int> root;

    public:
        Octree();
        lazy_node<int>& get_root();

        void set_default_field(Octree def);
        void add_point();

};
