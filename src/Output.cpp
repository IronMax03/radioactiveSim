#include "Output.h"
#include <fstream>
#include <iostream>

void write_particles_csv(const std::vector<Particle>& particles,
                          const std::string& filename) {
    // TODO output the information of each particle to a CSV file
    // the name of the file is given by the parameter 'filename'
    // The CSV file should have a header with the following columns:
    // x,y,z,alive,type
    // don't forget to close the file at the end

    std::ofstream tempStream(filename);

    if(tempStream.is_open())
    {
        tempStream << "x,y,z,alive,type\n";

        for(Particle p: particles)
            tempStream << p.x << "," << p.y << "," << p.z << "," << p.alive << "," << p.type << "\n";

        tempStream.close();
    }
    else
    {
        std::cout << "Error: Could not create or open " << filename << "." << std::endl;
        throw;
    }
}
