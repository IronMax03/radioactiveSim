## compile project
To compile the project, run `make` in `src/`.

## Structure of the code
**src/**
- **Particle.h/cpp:** properties of particles
- **Shielding.h/cpp:** defines the shielding
- **Material.h/cpp:** possible materials for the shielding
- **Source.h/cpp:** how the particles are emitted
- **ROI.h/cpp:** the Region Of Interest, a simple box
- **Simulation.h/cpp:** contains the simulation parameters (source, material
used, etc…)
- **Output.h/cpp:** handles the outputs of the simulation
- **GeometryOutput.h/cpp:** handles the outputs of boxes (ROI, Shielding,
Source)
- **utils.h:** useful functions, used only for random number generation for
now
- **main.cpp:** create and launch a Simulation object