#ifndef OUTPUT_H
#define OUTPUT_H

#include <vector>
#include <string>
#include "Particle.h"

void write_particles_csv(const std::vector<Particle>& particles,
                          const std::string& filename);

#endif
