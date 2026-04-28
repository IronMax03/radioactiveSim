/// @file Output.cpp
/// @brief Implementation of CSV output utilities for particle state persistence.

#include "Output.h"
#include <fstream>
#include <iostream>

/// @brief Write the state of a collection of particles to a CSV file.
///
/// Creates or overwrites @p filename and writes a header row followed by one
/// data row per particle with columns: @c x, @c y, @c z, @c alive, @c type.
///
/// @param particles The collection of particles to serialise.
/// @param filename  Path to the output CSV file.
/// @throws If the file cannot be opened an error message is printed to @c stdout
///         and the exception is re-thrown.
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
            tempStream << p.position.x << "," << p.position.y << "," << p.position.z << "," << p.alive << "," << p.type << "\n";

        tempStream.close();
    }
    else
    {
        std::cout << "Error: Could not create or open " << filename << "." << std::endl;
        throw;
    }
}
