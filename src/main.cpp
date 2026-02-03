#include "Simulation.h"
#include "Material.h"
#include "GeometryOutput.h"
#include <iostream>

int main() {
    std::cout << "Initializing Radiation Protection simulation" << std::endl;
    // Simulation parameters
    float timestep = 0.01f;
    unsigned numIterations = 5000;
    Material lead("Lead", 10);
    Shielding shield(-1, 1, -1, 1, 1, 2, lead);
    ROI roi(-0.5, 0.5, -0.5, 0.5, 3, 4);
    Source src(0.0, 0.0, 0.0, 5);

    // output boxes that won't change
    write_box_vtp(shield.xmin, shield.xmax,
                  shield.ymin, shield.ymax,
                  shield.zmin, shield.zmax,
                  "shielding.vtp");

    write_box_vtp(roi.xmin, roi.xmax,
                  roi.ymin, roi.ymax,
                  roi.zmin, roi.zmax,
                  "roi.vtp");
    
    // we draw a box for the source as well, even if it's not a geometry per se
    write_box_vtp(src.x - 0.1, src.x + 0.1,
                  src.y - 0.1, src.y + 0.1,
                  src.z - 0.4, src.z,
                  "source.vtp");
    
    // define simulation
    Simulation sim(timestep, numIterations, src, shield, roi);
    // print the parameters for the user
    sim.print();
    // and run it
    sim.run();

    // print the results
    std::cout << "Total # particles scored in ROI: " << sim.roi.count << std::endl;

    return 0;
}
