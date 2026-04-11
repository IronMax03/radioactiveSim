#include <iostream>
#include <cassert>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "dependencies/doctest.h"
#include "Particle.h"
#include "ROI.h"
#include "Output.h"
#include "Octree.h"

using namespace std;

TEST_CASE("Vector3 operations")
{
    vector3<double> v1{1.0, 2.0, 3.0};
    vector3<double> v2{4.0, 5.0, 6.0};

    CHECK(v1 == vector3<double>{1.0, 2.0, 3.0});
    CHECK(v2 == vector3<double>{4.0, 5.0, 6.0});
    CHECK(v1 + v2 == vector3<double>{5.0, 7.0, 9.0});
    CHECK(v1 - v2 == vector3<double>{-3.0, -3.0, -3.0});
    CHECK(v1 - v1 == vector3<double>{0.0, 0.0, 0.0});
    CHECK(v2 - v1 == vector3<double>{3.0, 3.0, 3.0});
    CHECK(v1 * 2.0 == vector3<double>{2.0, 4.0, 6.0});
    CHECK(2.0 * v1 == v1 * 2.0);
    CHECK(v1 / 2.0 == vector3<double>{0.5, 1.0, 1.5});
    CHECK_THROWS(v1 / 0.0);
    CHECK_THROWS(v2 / 0.0);
    CHECK(v1 * v2 == 32.0);
    CHECK(v1.norm() == std::sqrt(14.0));
}

TEST_CASE("Vector3 stream output")
{
    vector3<double> v{1.1, 2.0, 3.0};
    std::ostringstream oss;
    oss << v;
    CHECK(oss.str() == "(1.1,2,3)");

    vector3<int> v2{};
    oss.str("");
    oss << v2;
    CHECK(oss.str() == "(0,0,0)");
}


TEST_CASE("Particle movement") 
{
    Particle p(0,0,0,1,0,0, "proton ");

    p.move(0.01);
    CHECK(p.position == Particle(0.01,0,0,1,0,0, "proton ").position);

    for(size_t i = 0; i < 3; i++)
        p.move(0.01);
    
    CHECK(p.position == Particle(0.04,0,0,1,0,0, "proton ").position);
}

TEST_CASE("ROI contains") 
{
    ROI temp(0, 1, 0, -1, 0, 1);
    const Particle p(0.5, 0, 0.5, 0, 0, 0, "proton ");
    CHECK(temp.contains(p));
}



TEST_CASE("LazyNode Octree behavior") 
{
    lazy_node <int> n;

    for (size_t i = 0; i <= 7; i++)
    {
        CHECK_FALSE(n.is_child_inst(i));
        CHECK_FALSE(n.childs[i] != nullptr);
    }
    
    n.instantiate_child(0);
    n.instantiate_child(4);
    n.instantiate_child(7);

    CHECK(n.is_child_inst(0));

    n.get(0).instantiate_child(1);

    CHECK(n.is_child_inst(0));
    CHECK(n.is_child_inst(4));
    CHECK(n.is_child_inst(7));

    CHECK_FALSE(n.is_child_inst(1));
    CHECK_FALSE(n.is_child_inst(2));
    CHECK_FALSE(n.is_child_inst(3));
    CHECK_FALSE(n.is_child_inst(5));
    CHECK_FALSE(n.is_child_inst(6));

    CHECK(n.childs[0]->is_child_inst(1));
    CHECK(n.childs[0]->childs[1] != nullptr);

    n.get(2);
    CHECK(n.is_child_inst(2));
}

TEST_CASE("LazyNode Barnes-Hut functionality") 
{
    lazy_node <int> n;
    
    n.get(0).total_charge = 3;
    n.get(0).center_of_charge = vector3<int>{2, 0, 0};

    n.get(0).get(1).total_charge = 5;
    n.get(0).get(1).center_of_charge = vector3<int>{0, 2, 0};
}

TEST_CASE("LazyNode error handling") 
{
    lazy_node <int> n;
    CHECK_THROWS(n.instantiate_child(8));
    CHECK_THROWS(n.get(8));
}


TEST_CASE("Quadtree") 
{

}