#include <iostream>
#include <cassert>
#include "Particle.h"
#include "ROI.h"
#include "Output.h"
#include "Quadtree.h"

using namespace std;

int main()
{   
    // particules
    Particle p(0,0,0,1,0,0, "proton ");

    p.move(0.01);
    assert(p == Particle(0.01,0,0,1,0,0, "proton "));

    for(size_t i = 0; i < 3; i++)
        p.move(0.01);
    
    assert(p == Particle(0.04,0,0,1,0,0, "proton "));

    // ROI
    ROI temp(0, 1, 0, -1, 0, 1);
    assert(temp.contains(Particle(0.5, -0.5, 0.5,0,0,0, "positron")));


    // write_particles_csv
    write_particles_csv(vector<Particle>{Particle(0.01,0,0,1,0,0, "proton "), Particle(1,0,0,1,0,0, "proton ")}, "OUTPUT.csv");

    // LazyNode
    LazyNode<int> n;
    n.value = 4;
    assert(n.read() == n.value);
    assert(!(n.is_eval_00() || n.is_eval_01() || n.is_eval_10() || n.is_eval_11()));

    n.get_00().value = 0;
    assert(n.is_eval_00());
    assert(n.get_00().read() == 0);

    n.get_01().value = 1;
    assert(n.is_eval_01());
    assert(n.get_01().read() == 1);

    n.get_10().value = 2;
    assert(n.is_eval_10());
    assert(n.get_10().read() == 2);

    n.get_11().value = 3;
    assert(n.is_eval_11());
    assert(n.get_11().read() == 3);

    n.get_11().get_00().value = -1;
    assert(n.get_11().get_00().read() == -1);


    // quadtree
    Quadtree q;
    q.get_root().value = 3;
}