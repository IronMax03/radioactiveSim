# Radioactive Shielding Simulation
This project simulates the shielding of radioactive particles.

## Using the simulation
Click [here](./doc/html/index.html) to access the documentation of the project.

To start the simulation run the following command from the ``src/`` directory:
``` bash
make run
```

## Contributing

### Bug reporting
If you find a bug, please report it by opening an issue using the Bug report template. Note that the name of the issue will be automatically formatted.

### compile project
To compile the project, run `make` in `src/`.  
To run the simulation:
``` bash
./shielding_sim
```
### test project

To run test `doctest` **v2.5.1** must be installed in `src/dependencies`.
This can be done by dragging and dropping the `doctest.h` file from the official [doctest repository](https://github.com/doctest/doctest) or by running the following command from `src/dependencies`:

``` bash
curl -LO https://raw.githubusercontent.com/doctest/doctest/refs/heads/master/doctest/doctest.h
```

When this is done run the following commend from `src` to compile and run tests.
``` bash
make test
```

### Generate documentation
In order to generate documentation, `doxygen` must be installed on the system. Then run the following command from the repository root:
``` bash
doxygen Doxyfile
```

### Structure of the code
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