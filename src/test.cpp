#include <iostream>
#include <cassert>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "dependencies/doctest.h"
#include "Particle.h"
#include "ROI.h"
#include "Output.h"
//#include "Quadtree.h"

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
    CHECK(v1 * v2 == 32.0);
    CHECK(v1.norm() == std::sqrt(14.0));
}

TEST_CASE("Vector3 stream output")
{
    vector3<double> v{1.1, 2.0, 3.0};
    std::ostringstream oss;
    oss << v;
    CHECK(oss.str() == "(1.1,2,3)");
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


/*
TEST_CASE("LazyNode basic behavior") 
{
    LazyNode<int> n;
    n.value = 4;

    CHECK(n.read() == n.value);
    CHECK_FALSE(n.is_eval_00() || n.is_eval_01() || n.is_eval_10() || n.is_eval_11());

    n.get_00().value = 0;
    CHECK(n.is_eval_00());
    CHECK(n.get_00().read() == 0);

    n.get_01().value = 1;
    CHECK(n.is_eval_01());
    CHECK(n.get_01().read() == 1);

    n.get_10().value = 2;
    CHECK(n.is_eval_10());
    CHECK(n.get_10().read() == 2);

    n.get_11().value = 3;
    CHECK(n.is_eval_11());
    CHECK(n.get_11().read() == 3);

    n.get_11().get_00().value = -1;
    CHECK(n.get_11().get_00().read() == -1);
}

TEST_CASE("Quadtree root") 
{
    Quadtree q;
    q.get_root().value = 3;

    CHECK(q.get_root().value == 3);
}*/