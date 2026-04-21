/**
 * @file test.cpp
 * @brief This file contains unit tests for the utility functions and data structures used in the simulation.
 * The tests are implemented using the doctest framework, which is a lightweight and easy-to-use C++ testing framework. 
 * @note doctest.h must be downloaded from the official doctest repository and placed in the dependencies folder for this file to compile and run correctly.
*/

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "dependencies/doctest.h"
#include "Particle.h"
#include "ROI.h"
#include "Octree.h"

using namespace std;


TEST_SUITE("Vector3 struct")
{
    TEST_CASE("vector operations")
    {
        vector3<double> v1{1.0, 2.0, 3.0};
        vector3<double> v2{4.0, 5.0, 6.0};

        SUBCASE("comparison equality")
        {
            CHECK(v1 == vector3<double>{1.0, 2.0, 3.0});
            CHECK(v2 == vector3<double>{4.0, 5.0, 6.0});
        }

        SUBCASE("addition and subtraction")
        {
            CHECK_EQ(v1 + v2, vector3<double>{5.0, 7.0, 9.0});
            CHECK(v2 + v1 == v1 + v2);
            CHECK_EQ(v1 - v2, vector3<double>{-3.0, -3.0, -3.0});
            CHECK_EQ(v1 - v1, vector3<double>{0.0, 0.0, 0.0});
            CHECK_EQ(v2 - v1, vector3<double>{3.0, 3.0, 3.0});
        }

        SUBCASE("scalar multiplication and division")
        {
            CHECK_EQ(v1 * 2.0, vector3<double>{2.0, 4.0, 6.0});
            CHECK(2.0 * v1 == v1 * 2.0);
            CHECK_EQ(v1 / 2.0, vector3<double>{0.5, 1.0, 1.5});
            CHECK_THROWS(v1 / 0.0);
            CHECK_THROWS(v2 / 0.0);
        }

        SUBCASE("dot product") 
        { 
            CHECK_EQ(v1 * v2, 32.0); 
            CHECK(v1 * v2 == v2 * v1); 
        }

        SUBCASE("norm and normalize")
        {
            CHECK_EQ(v1.norm(), std::sqrt(14.0));
            vector3<double> v3{0.0, 0.0, 0.0};
            CHECK_THROWS(v3.normalize());
            CHECK_EQ(v3.norm(), 0.0);
            v3.x = 4.0;
            CHECK_EQ(v3.norm(), 4.0);
            v3.normalize();
            CHECK_EQ(v3.norm(), 1.0);
        }
    }

    TEST_CASE("stream output")
    {
        vector3<double> v{1.1, 2.0, 3.0};
        std::ostringstream oss;
        oss << v;
        CHECK_EQ(oss.str(), "(1.1,2,3)");

        vector3<int> v2{};
        oss.str("");
        oss << v2;
        CHECK_EQ(oss.str(), "(0,0,0)");
    }
}

TEST_SUITE("Particle")
{
    TEST_CASE("Particle Constructor") 
    {
        Particle p(1.0, 2.0, 3.0, 0.1, 0.2, 0.3, "proton");
        CHECK_EQ(p.position, vector3<double>{1.0, 2.0, 3.0});
        CHECK_EQ(p.velocity, vector3<double>{0.1, 0.2, 0.3});
        CHECK_EQ(p.electric_charge, 1);
        CHECK_EQ(p.type, "proton");
        CHECK_EQ(p.distance_in_shield, 0.0);
        CHECK(p.alive);

        CHECK_THROWS(Particle(1.0, 2.0, 3.0, 0.1, 0.2, 0.3, "invalid_type"));
        CHECK_EQ(Particle(1.0, 2.0, 3.0, 0.1, 0.2, 0.3, "neutron").electric_charge, 0);
        CHECK_EQ(Particle(1.0, 2.0, 3.0, 0.1, 0.2, 0.3, "electron").electric_charge, -1);
    }


    TEST_CASE("Particle.move(double dt)") 
    {
        Particle p(0,0,0,1,0,0, "proton");

        p.move(0.01);
        CHECK(p.position == Particle(0.01,0,0,1,0,0, "proton").position);

        for(size_t i = 0; i < 3; i++)
            p.move(0.01);
        
        CHECK(p.position == Particle(0.04,0,0,1,0,0, "proton").position);
        CHECK_THROWS(p.move(-0.05));
        CHECK_THROWS(p.move(0));
    }
}

TEST_SUITE("ROI")
{
    TEST_CASE("contains") 
    {
        ROI temp(0, 1, 0, -1, 0, 1);
        const Particle p(0.5, 0, 0.5, 0, 0, 0, "proton");
        CHECK(temp.contains(p));
    }
}


TEST_SUITE("electrodynamic implementation via Barnes-Hut method")
{


    TEST_CASE("LazyNode Octree behavior") 
    {
        lazy_node <int> n;

        SUBCASE("child instantiation")
        {
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

        SUBCASE("child access")
        {
            CHECK_THROWS(n.get(8));
            CHECK_THROWS(n.get(-1));
        }

        SUBCASE("spatial subdivision (child_index function)")
        {
            lazy_node <double> d;
            d.bounding_box.center = vector3<double>{0.0, 0.0, 0.0};
            d.bounding_box.length = 2.0;

            CHECK_EQ(d.child_index(vector3<double>{-1.0, -1.0, -1.0}), 0u);
            CHECK_EQ(d.child_index(vector3<double>{1.0, -1.0, -1.0}), 1u);
            CHECK_EQ(d.child_index(vector3<double>{-1.0, 1.0, -1.0}), 2u);
            CHECK_EQ(d.child_index(vector3<double>{-1.0, -1.0, 1.0}), 4u);
            CHECK_EQ(d.child_index(vector3<double>{1.0, 1.0, -1.0}), 3u);
            CHECK_EQ(d.child_index(vector3<double>{-1.0, 1.0, 1.0}), 6u);
            CHECK_EQ(d.child_index(vector3<double>{1.0, -1.0, 1.0}), 5u);
            CHECK_EQ(d.child_index(vector3<double>{1.0, 1.0, 1.0}), 7u);

            CHECK_EQ(d.child_index(vector3<double>{0.0, 0.0, 0.0}), 7u);

            CHECK_THROWS(d.child_index(vector3<double>{-2.0, 0.0, 0.0}));
            CHECK_THROWS(d.child_index(vector3<double>{0.0, -2.0, 0.0}));
            CHECK_THROWS(d.child_index(vector3<double>{0.0, 0.0, -2.0}));
            CHECK_THROWS(d.child_index(vector3<double>{2.0, 0.0, 0.0}));
            CHECK_THROWS(d.child_index(vector3<double>{0.0, 2.0, 0.0}));
            CHECK_THROWS(d.child_index(vector3<double>{0.0, 0.0, 2.0}));
        }
    }

    TEST_CASE("LazyNode Barnes-Hut functionality") 
    {
        lazy_node <int> n;
        lazy_node <double> d;

        SUBCASE("child value assignment and retrieval")
        {
            n.get(0).total_charge = 3;
            CHECK_EQ(n.get(0).total_charge, 3);
            n.get(0).center_of_charge = vector3<int>{2, 0, 0};
            CHECK_EQ(n.get(0).center_of_charge, vector3<int>{2, 0, 0});

            n.get(0).get(1).total_charge = 5;
            n.get(0).get(1).center_of_charge = vector3<int>{0, 2, 0};
            CHECK_EQ(n.get(0).get(1).center_of_charge, vector3<int>{0, 2, 0});

            d.get(0).total_charge = 3;
            CHECK_EQ(d.get(0).total_charge, 3);
            d.get(0).center_of_charge = vector3<double>{2.5, 0.0, 0.0};
            CHECK_EQ(d.get(0).center_of_charge, vector3<double>{2.5, 0.0, 0.0}); 
        }

        SUBCASE("Barnes-Hut criteria")
        {
            n.bounding_box.length = 1.0;
            n.center_of_charge = vector3<int>{0, 0, 0};

            CHECK_FALSE(n.BH_criteria(1.0, 0.0, 0.0, 1.0));
            CHECK(n.BH_criteria(1.0, 0.0, 0.0, 2.0));
            CHECK(n.BH_criteria(2.0, 0.0, 0.0, 2.5));

            d.center_of_charge = vector3<double>{4.2, 4, -7.2};
            d.bounding_box.length = 1.93;
            CHECK(d.BH_criteria(37, 69, 76, 1.5));
            CHECK_FALSE(d.BH_criteria(37, 69, 76, 0.005));
        }

        SUBCASE("Center of charge evaluation")
        {
            d.get(0).get(0).total_charge = 3;
            d.get(0).get(0).center_of_charge = vector3<double>{2.0, 0.0, 0.0};

            d.get(0).get(1).total_charge = 5;
            d.get(0).get(1).center_of_charge = vector3<double>{0.0, 2.0, 0.0};

            d.get(0).eval();

            CHECK_EQ(d.get(0).total_charge, 8);
            CHECK_EQ(d.get(0).center_of_charge, vector3<double>{(3*2.0 + 5*0.0)/8, (3*0.0 + 5*2.0)/8, 0.0});

            d.get(3).eval();
            CHECK_EQ(d.get(3).total_charge, 0);
            CHECK_EQ(d.get(3).center_of_charge, vector3<double>{0.0, 0.0, 0.0});
        }
    }

    TEST_CASE("LazyNode error handling") 
    {
        lazy_node <int> n;
        CHECK_THROWS(n.instantiate_child(8));
        CHECK_THROWS(n.get(8));
        CHECK_THROWS(n.get(-1));

        n.instantiate_child(2);
        CHECK_THROWS(n.instantiate_child(8));
    }

    TEST_CASE("Quadtree") 
    {
        SUBCASE("Add particle to octree")
        {
            Octree o(vector3<double>{0, 0, 0}, vector3<double>{10, 10, 10});
            o.add_particle(Particle(1.0, 1.0, 1.0, 0.0, 0.0, 0.0, "proton"));

            CHECK_EQ(o.get_root().particle->position, vector3<double>{1.0, 1.0, 1.0});
            CHECK_EQ(o.get_root().particle->electric_charge, 1);

            o.add_particle(Particle(9.0, 9.0, 9.0, 0.0, 0.0, 0.0, "electron"));

            // check electron
            CHECK(o.get_root().is_child_inst(7));
            CHECK(o.get_root().get(7).is_leaf());
            CHECK_EQ(o.get_root().get(7).particle->position, vector3<double>{9.0, 9.0, 9.0});
            CHECK_EQ(o.get_root().get(7).particle->electric_charge, -1);
            CHECK_EQ(o.get_root().get(7).particle->type, "electron");
            
            // check proton
            CHECK(o.get_root().is_child_inst(0));
            CHECK(o.get_root().get(0).is_leaf());
            CHECK_EQ(o.get_root().get(0).particle->position, vector3<double>{1.0, 1.0, 1.0});
            CHECK_EQ(o.get_root().get(0).particle->electric_charge, 1);
            CHECK_EQ(o.get_root().get(0).particle->type, "proton");
        }
    }
}