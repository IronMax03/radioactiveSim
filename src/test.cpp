/// @file test.cpp
/// @brief This file contains unit tests for the utility functions and data structures used in the simulation.
/// @note doctest.h must be downloaded from the official doctest repository and placed in the dependencies folder for this file to compile and run correctly.
/// @author Maximilien Notz

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "dependencies/doctest.h"
#include "Particle.h"
#include "ROI.h"
#include "Octree.h"
#include <cmath>

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

    TEST_CASE("stream output & string conversion")
    {
        vector3<double> v{1.1, 2.0, 3.0};
        std::ostringstream oss;
        oss << v;
        CHECK_EQ(oss.str(), "(1.1,2,3)");
        CHECK(oss.str() == v.to_string());

        vector3<int> v2{};
        oss.str("");
        oss << v2;
        CHECK_EQ(oss.str(), "(0,0,0)");
        CHECK(oss.str() == v2.to_string());
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
        n.bounding_box.center = vector3<int>{0, 0, 0}; 
        n.bounding_box.length = 8;

        SUBCASE("child instantiation")
        {
            for (size_t i = 0; i <= 7; i++)
            {
                CHECK_FALSE(n.is_child_inst(i));
                CHECK_FALSE(n.childs[i] != nullptr);
            }
            
            n.instantiate_child(0);
            CHECK_EQ(n.get(0).bounding_box.center, vector3<int>{-2, -2, -2});
            CHECK_EQ(n.get(0).bounding_box.length, 4);
            n.instantiate_child(4);
            CHECK_EQ(n.get(4).bounding_box.center, vector3<int>{-2, -2, 2});
            CHECK_EQ(n.get(4).bounding_box.length, 4);
            n.instantiate_child(7);
            CHECK_EQ(n.get(7).bounding_box.center, vector3<int>{2, 2, 2});
            CHECK_EQ(n.get(7).bounding_box.length, 4);

            CHECK(n.is_child_inst(0));

            n.get(0).instantiate_child(1);
            CHECK_EQ(n.get(0).get(1).bounding_box.center, vector3<int>{-1, -3, -3});
            CHECK_EQ(n.get(0).get(1).bounding_box.length, 2);

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
            CHECK_EQ(d.get(0).center_of_charge, vector3<double> {(3*2.0 + 5*0.0)/8, (3*0.0 + 5*2.0)/8, 0.0});

            d.get(3).eval();
            CHECK_EQ(d.get(3).total_charge, 0);
            CHECK_EQ(d.get(3).center_of_charge, vector3<double> {0.0, 0.0, 0.0});
        }

        SUBCASE("LazyNode error handling")
        {
            lazy_node <int> n;
            CHECK_THROWS(n.instantiate_child(8));
            CHECK_THROWS(n.get(8));
            CHECK_THROWS(n.get(-1));

            n.instantiate_child(2);
            CHECK_THROWS(n.instantiate_child(8));
        }
    }

    TEST_CASE("Quadtree")
    {
        SUBCASE("Add particle to octree")
        {
            Octree o(vector3<double>{0, 0, 0}, vector3<double>{10, 10, 10});
            o.add_particle(Particle(1.0, 1.0, 1.0, 0.0, 0.0, 0.0, "proton"));

            CHECK_EQ(o.get_root().particle->position, vector3<double>{1.0, 1.0, 1.0});
            CHECK_EQ(o.get_root().particle->electric_charge, 1);

            o.add_particle(Particle(9.0, 9.0, 9.0, 0.0, 0.0, 0.0, "neutron"));

            // check neutron
            CHECK(o.get_root().is_child_inst(0b111));
            CHECK(o.get_root().get(0b111).is_leaf());
            CHECK_EQ(o.get_root().get(0b111).particle->position, vector3<double>{9.0, 9.0, 9.0});
            CHECK_EQ(o.get_root().get(0b111).particle->electric_charge, 0);
            CHECK_EQ(o.get_root().get(0b111).particle->type, "neutron");
            
            // check proton
            CHECK(o.get_root().particle == nullptr);
            CHECK(o.get_root().is_child_inst(0b000));
            CHECK(o.get_root().get(0b000).is_leaf());
            CHECK_EQ(o.get_root().get(0b000).particle->position, vector3<double>{1.0, 1.0, 1.0});
            CHECK_EQ(o.get_root().get(0b000).particle->electric_charge, 1);
            CHECK_EQ(o.get_root().get(0b000).particle->type, "proton");
        }

        SUBCASE("Build octree from vector of particles")
        {
            Octree o(vector3<double>{0, 0, 0}, vector3<double>{10, 10, 10});
            std::vector<Particle> particles = {
                Particle(1.0, 1.0, 1.0, 0.0, 0.0, 0.0, "proton"),
                Particle(9.0, 9.0, 9.0, 0.0, 0.0, 0.0, "neutron"),
                Particle(2.5, 5.0, 5.0, 0.0, 0.0, 0.0, "neutron")
            };

            o.build(particles);
            CHECK(o.get_root().is_child_inst(0b000));
            CHECK(o.get_root().is_child_inst(0b111));
            CHECK(o.get_root().is_child_inst(0b110));
            std::cout << "Octree built successfully from vector of particles." << std::endl;

            CHECK_EQ(o.get_root().read(0b000).particle->position, vector3<double>{1.0, 1.0, 1.0});
            CHECK_EQ(o.get_root().read(0b000).particle->electric_charge, 1);
            CHECK_EQ(o.get_root().read(0b000).particle->type, "proton");
            
            CHECK_EQ(o.get_root().read(0b111).particle->position, vector3<double>{9.0, 9.0, 9.0});
            CHECK_EQ(o.get_root().read(0b111).particle->electric_charge, 0);
            CHECK_EQ(o.get_root().read(0b111).particle->type, "neutron");

            CHECK_EQ(o.get_root().read(0b110).particle->position, vector3<double>{2.5, 5.0, 5.0});
            CHECK_EQ(o.get_root().read(0b110).particle->electric_charge, 0);
            CHECK_EQ(o.get_root().read(0b110).particle->type, "neutron");
        }
    }

    TEST_CASE("Barnes-Hut force calculation")
    {
        // Coulomb constant – must match the value in Octree.cpp.
        constexpr double k_e = 0.13794;

        SUBCASE("Two protons – axis-aligned repulsion")
        {
            // Two +1 charges on the x-axis, 2 m apart.
            //   r = p1.pos - p2.pos = (2,0,0),  |r| = 2
            //   F on p1 = k_e * q1 * q2 * r / |r|^3 = k_e * (2,0,0) / 8 = (k_e/4, 0, 0)
            Octree o(vector3<double>{-10,-10,-10}, vector3<double>{10,10,10});
            Particle p1( 1.0, 0.0, 0.0, 0,0,0, "proton");
            Particle p2(-1.0, 0.0, 0.0, 0,0,0, "proton");
            o.add_particle(p1);
            o.add_particle(p2);

            o.set_theta(0.0f); // exact – always recurse to leaves
            o.calc_force(p1);

            double expected_fx = k_e / 4.0;
            CHECK(p1.acceleration.x == doctest::Approx(expected_fx).epsilon(1e-6));
            CHECK(p1.acceleration.y == doctest::Approx(0.0));
            CHECK(p1.acceleration.z == doctest::Approx(0.0));
        }

        SUBCASE("Neutral particle produces zero force")
        {
            Octree o(vector3<double>{-10,-10,-10}, vector3<double>{10,10,10});
            Particle proton( 1.0, 0.0, 0.0, 0,0,0, "proton");
            Particle neutron(-1.0, 0.0, 0.0, 0,0,0, "neutron"); // charge = 0
            o.add_particle(proton);
            o.add_particle(neutron);

            // Force on the proton from a neutron should be zero.
            o.set_theta(0.0f);
            o.calc_force(proton);
            CHECK(proton.acceleration.x == doctest::Approx(0.0));
            CHECK(proton.acceleration.y == doctest::Approx(0.0));
            CHECK(proton.acceleration.z == doctest::Approx(0.0));

            // Force on the neutron (q_target = 0) should also be zero.
            o.calc_force(neutron);
            CHECK(neutron.acceleration.x == doctest::Approx(0.0));
            CHECK(neutron.acceleration.y == doctest::Approx(0.0));
            CHECK(neutron.acceleration.z == doctest::Approx(0.0));
        }

        SUBCASE("Newton's third law – equal and opposite forces")
        {
            // Particles in distinct octants so the tree stores both correctly.
            Octree o(vector3<double>{-10,-10,-10}, vector3<double>{10,10,10});
            Particle p1( 2.0,  3.0,  1.0, 0,0,0, "proton"); // octant 7
            Particle p2(-5.0, -7.0, -4.0, 0,0,0, "proton"); // octant 0
            o.add_particle(p1);
            o.add_particle(p2);
            o.set_theta(0.0f);

            Particle p1_copy = p1;
            Particle p2_copy = p2;

            o.calc_force(p1_copy);
            o.calc_force(p2_copy);

            CHECK(p1_copy.acceleration.x == doctest::Approx(-p2_copy.acceleration.x).epsilon(1e-6));
            CHECK(p1_copy.acceleration.y == doctest::Approx(-p2_copy.acceleration.y).epsilon(1e-6));
            CHECK(p1_copy.acceleration.z == doctest::Approx(-p2_copy.acceleration.z).epsilon(1e-6));
        }

        SUBCASE("Three-body superposition")
        {
            // Each particle in a distinct octant so the tree stores all three.
            //   p1 (-1,-1,-1) → octant 0   (target)
            //   p2 ( 3,-1,-1) → octant 1
            //   p3 (-1, 4,-1) → octant 2
            Octree o(vector3<double>{-20,-20,-20}, vector3<double>{20,20,20});
            Particle p1(-1.0,-1.0,-1.0, 0,0,0, "proton");
            Particle p2( 3.0,-1.0,-1.0, 0,0,0, "proton");
            Particle p3(-1.0, 4.0,-1.0, 0,0,0, "proton");
            o.add_particle(p1);
            o.add_particle(p2);
            o.add_particle(p3);
            o.set_theta(0.0f);

            o.calc_force(p1);

            // F from p2: r = (-1-3, 0, 0) = (-4,0,0), |r|=4  =>  F = k_e*(-4,0,0)/64 = (-k_e/16, 0, 0)
            // F from p3: r = (0, -1-4, 0) = (0,-5,0), |r|=5  =>  F = k_e*(0,-5,0)/125 = (0, -k_e/25, 0)
            double expected_fx = -k_e / 16.0;
            double expected_fy = -k_e / 25.0;

            CHECK(p1.acceleration.x == doctest::Approx(expected_fx).epsilon(1e-6));
            CHECK(p1.acceleration.y == doctest::Approx(expected_fy).epsilon(1e-6));
            CHECK(p1.acceleration.z == doctest::Approx(0.0));
        }

        SUBCASE("Diagonal separation – 3D force direction")
        {
            // Two protons along the body diagonal: (1,1,1) and (-1,-1,-1).
            // r = (2,2,2), |r| = 2*sqrt(3), |r|^3 = 24*sqrt(3)
            Octree o(vector3<double>{-10,-10,-10}, vector3<double>{10,10,10});
            Particle p1( 1.0, 1.0, 1.0, 0,0,0, "proton");
            Particle p2(-1.0,-1.0,-1.0, 0,0,0, "proton");
            o.add_particle(p1);
            o.add_particle(p2);
            o.set_theta(0.0f);

            o.calc_force(p1);

            double dist3 = std::pow(2.0 * std::sqrt(3.0), 3);
            double expected = k_e * 2.0 / dist3; // all three components equal

            CHECK(p1.acceleration.x == doctest::Approx(expected).epsilon(1e-6));
            CHECK(p1.acceleration.y == doctest::Approx(expected).epsilon(1e-6));
            CHECK(p1.acceleration.z == doctest::Approx(expected).epsilon(1e-6));
        }

        SUBCASE("Single particle – no self-interaction")
        {
            Octree o(vector3<double>{-10,-10,-10}, vector3<double>{10,10,10});
            Particle p(3.0, 4.0, 5.0, 0,0,0, "proton");
            o.add_particle(p);
            o.set_theta(0.0f);

            o.calc_force(p);

            CHECK(p.acceleration.x == doctest::Approx(0.0));
            CHECK(p.acceleration.y == doctest::Approx(0.0));
            CHECK(p.acceleration.z == doctest::Approx(0.0));
        }

        SUBCASE("Inverse-square distance scaling")
        {
            // Source at negative x, targets at positive x → different octants.
            // o1: distance = 4,  o2: distance = 8  →  |F(4)| / |F(8)| = 4
            Octree o1(vector3<double>{-50,-50,-50}, vector3<double>{50,50,50});
            Octree o2(vector3<double>{-50,-50,-50}, vector3<double>{50,50,50});

            Particle source(-1.0, 0,0, 0,0,0, "proton"); // octant 6
            Particle near_p( 3.0, 0,0, 0,0,0, "proton"); // octant 7, dist 4
            Particle far_p(  7.0, 0,0, 0,0,0, "proton"); // octant 7, dist 8

            o1.add_particle(source);  o1.add_particle(near_p);
            o2.add_particle(Particle(-1.0,0,0, 0,0,0,"proton"));
            o2.add_particle(far_p);

            o1.set_theta(0.0f);
            o2.set_theta(0.0f);

            Particle t1 = near_p;
            Particle t2 = far_p;
            o1.calc_force(t1);
            o2.calc_force(t2);

            double ratio = t1.acceleration.x / t2.acceleration.x;
            CHECK(ratio == doctest::Approx(4.0).epsilon(1e-6));
        }

        SUBCASE("Barnes-Hut approximation converges toward exact solution")
        {
            // Cluster of four protons spread across distinct octants, with a
            // distant target in yet another octant.  The BH approximation
            // (theta = 0.5) should be close to the exact result (theta = 0).
            Octree exact_tree(vector3<double>{-50,-50,-50}, vector3<double>{50,50,50});
            Octree approx_tree(vector3<double>{-50,-50,-50}, vector3<double>{50,50,50});

            Particle target(0.0, -1.0, -40.0, 0,0,0, "proton"); // octant 1

            std::vector<Particle> cluster = {
                Particle( 1.0,  1.0,  1.0, 0,0,0, "proton"), // octant 7
                Particle(-1.0,  1.0,  1.0, 0,0,0, "proton"), // octant 6
                Particle( 1.0, -1.0,  1.0, 0,0,0, "proton"), // octant 5
                Particle( 1.0,  1.0, -1.0, 0,0,0, "proton")  // octant 3
            };

            exact_tree.add_particle(target);
            approx_tree.add_particle(target);
            for (const auto& p : cluster)
            {
                exact_tree.add_particle(p);
                approx_tree.add_particle(p);
            }

            Particle t_exact  = target;
            Particle t_approx = target;

            exact_tree.set_theta(0.0f);
            exact_tree.calc_force(t_exact);

            approx_tree.set_theta(0.5f);
            approx_tree.calc_force(t_approx);

            // Well-separated cluster → approximation within ~5 % of exact.
            CHECK(t_approx.acceleration.x == doctest::Approx(t_exact.acceleration.x).epsilon(0.05));
            CHECK(t_approx.acceleration.y == doctest::Approx(t_exact.acceleration.y).epsilon(0.05));
            CHECK(t_approx.acceleration.z == doctest::Approx(t_exact.acceleration.z).epsilon(0.05));
        }
    }

    TEST_CASE("Same-octant particle insertion (add_particle bugfix)")
    {
        // Regression: two particles in the same octant used to overwrite each
        // other because the push-down used direct assignment.  After the fix
        // both must survive in distinct leaves.
        SUBCASE("Two particles, same root octant")
        {
            Octree o(vector3<double>{-10,-10,-10}, vector3<double>{10,10,10});
            // Both positions have all coords ≥ 0 → morton index 7 at root level.
            o.add_particle(Particle(1.0, 1.0, 1.0, 0,0,0, "proton"));
            o.add_particle(Particle(8.0, 8.0, 8.0, 0,0,0, "proton"));

            // Root should no longer hold a particle — both must have been pushed down.
            CHECK(o.get_root().particle == nullptr);
            CHECK_FALSE(o.get_root().is_leaf());

            // Compute force on the first particle — should be non-zero (repulsion).
            Particle target(1.0, 1.0, 1.0, 0,0,0, "proton");
            o.set_theta(0.0f);
            o.calc_force(target);

            // Force must point away from (8,8,8), i.e. all components negative.
            CHECK(target.acceleration.x < 0);
            CHECK(target.acceleration.y < 0);
            CHECK(target.acceleration.z < 0);
        }

        SUBCASE("Three particles, all same root octant")
        {
            Octree o(vector3<double>{-10,-10,-10}, vector3<double>{10,10,10});
            o.add_particle(Particle(1.0, 1.0, 1.0, 0,0,0, "proton"));
            o.add_particle(Particle(2.0, 2.0, 2.0, 0,0,0, "proton"));
            o.add_particle(Particle(8.0, 8.0, 8.0, 0,0,0, "proton"));

            CHECK(o.get_root().particle == nullptr);

            // Force on a copy of the first particle should reflect TWO repulsive sources.
            Particle target(1.0, 1.0, 1.0, 0,0,0, "proton");
            o.set_theta(0.0f);
            o.calc_force(target);

            // Force must be non-zero and all components negative (both sources at larger coords).
            CHECK(target.acceleration.x < 0);
            CHECK(target.acceleration.y < 0);
            CHECK(target.acceleration.z < 0);

            // Compare with a tree containing only the (2,2,2) source — the three-body
            // force must be strictly larger in magnitude.
            Octree o_pair(vector3<double>{-10,-10,-10}, vector3<double>{10,10,10});
            o_pair.add_particle(Particle(1.0, 1.0, 1.0, 0,0,0, "proton"));
            o_pair.add_particle(Particle(2.0, 2.0, 2.0, 0,0,0, "proton"));
            o_pair.set_theta(0.0f);

            Particle target2(1.0, 1.0, 1.0, 0,0,0, "proton");
            o_pair.calc_force(target2);

            double mag_three = target.acceleration.norm();
            double mag_two   = target2.acceleration.norm();
            CHECK(mag_three > mag_two);
        }

        SUBCASE("Same-octant Newton's third law")
        {
            // Now that the bug is fixed, same-octant pairs should obey F12 = -F21.
            Octree o(vector3<double>{-10,-10,-10}, vector3<double>{10,10,10});
            Particle p1(2.0, 3.0, 1.0, 0,0,0, "proton");
            Particle p2(5.0, 7.0, 4.0, 0,0,0, "proton");
            o.add_particle(p1);
            o.add_particle(p2);
            o.set_theta(0.0f);

            Particle p1c = p1, p2c = p2;
            o.calc_force(p1c);
            o.calc_force(p2c);

            CHECK(p1c.acceleration.x == doctest::Approx(-p2c.acceleration.x).epsilon(1e-6));
            CHECK(p1c.acceleration.y == doctest::Approx(-p2c.acceleration.y).epsilon(1e-6));
            CHECK(p1c.acceleration.z == doctest::Approx(-p2c.acceleration.z).epsilon(1e-6));
        }
    }
}