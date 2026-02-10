#include <iostream>
#include <cassert>
#include "Particle.h"
#include "ROI.h"

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

}