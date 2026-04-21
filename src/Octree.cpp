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

lazy_node<nt>& Octree::get_root() { return root; } //! need to be changed 

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

void Octree::add_particle(const Particle& p) { add_particle(p, this->root); }

/*
void Octree::build(const lazy_node<nt>& node, const std::vector<Particle>& particles)
{

}*/
