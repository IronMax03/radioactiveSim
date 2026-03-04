#pragma once

template<typename T>
struct LazyNode
{
    T value;
    unique_ptr<LazyNode> childs[4];

    /// @brief Insures the variable is accessed without been modified.
    inline T read() const { return value; }

    /// @brief Return True if bottom left child is evaluated (exist).
    inline bool is_eval_00() const { return childs[0] != nullptr; }
    /// @brief Return True if bottom right child is evaluated (exist).
    inline bool is_eval_01() const { return childs[1] != nullptr; }
    /// @brief Return True if top left child is evaluated (exist).
    inline bool is_eval_10() const { return childs[2] != nullptr; }
    /// @brief Return True if top right child is evaluated (exist).
    inline bool is_eval_11() const { return childs[3] != nullptr; }

    #define GET_CHILD(n) {\
    if (childs[n] == nullptr)\
        childs[n] = make_unique<LazyNode<T>>();\
    return *childs[n];\
    }

    /// @brief Return the bottom left child.
    inline LazyNode& get_00() { GET_CHILD(0) }
    /// @brief Return the bottom right child.
    inline LazyNode& get_01() { GET_CHILD(1) }
    /// @brief Return the top left child.
    inline LazyNode& get_10() { GET_CHILD(2) }
    /// @brief Return the top right child.
    inline LazyNode& get_11() { GET_CHILD(3) }

    #undef GET_CHILD
};

template<typename T>
class Quadtree
{
    private:
        LazyNode root;
    public:
        Quadtree();
        ~Quadtree();

        LazyNode<T>& get_root() const;
};