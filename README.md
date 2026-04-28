# Radioactive Shielding Simulation

A 3-D radioactive particle transport simulator developed for **COSC 1560 – Computer Programming II**.  
The program models the emission of particles from a radioactive point source, their propagation through a configurable shielding volume, and their scoring inside a user-defined Region of Interest (ROI).  
Coulomb interactions between particles are approximated with the [Barnes-Hut algorithm](https://en.wikipedia.org/wiki/Barnes%E2%80%93Hut_simulation) via an octree spatial index.

The simulation outputs data in formats compatible with **[ParaView](https://www.paraview.org/)**, a powerful open-source tool for scientific visualization, allowing full 3-D inspection of particle trajectories, geometry, and simulation state over time.

---

## Table of Contents

1. [Project File Structure](#project-file-structure)
2. [Prerequisites](#prerequisites)
3. [Running the Simulation](#running-the-simulation)
4. [Running the Tests](#running-the-tests)
5. [Makefile Reference](#makefile-reference)
6. [ParaView Visualization](#paraview-visualization)
7. [API Documentation](#api-documentation)
8. [Bug Reporting](#bug-reporting)

---

---

## Project File Structure

```
radioactiveSim/
├── Doxyfile              # Doxygen configuration for API documentation generation
├── Makefile              # Root-level wrapper that forwards targets to src/makefile
├── README.md
├── doc/
│   ├── html/             # Generated HTML documentation (produced by `make gen-doc`)
│   └── images&animations/ # Images and animations used in the documentation
└── src/
    ├── makefile          # Primary build system
    ├── main.cpp          # Entry point – creates and launches a Simulation object
    ├── Simulation.h/cpp  # Top-level driver: owns source, shielding, ROI, and particle list
    ├── Particle.h/cpp    # Particle data (position, velocity, charge, mass, …)
    ├── Source.h/cpp      # Radioactive point source – controls emission rate and direction
    ├── Shielding.h/cpp   # Shielding volume – attenuates particles based on material
    ├── Material.h        # Material definitions (density, attenuation coefficients, …)
    ├── ROI.h/cpp         # Region of Interest – axis-aligned box used for scoring
    ├── Octree.h/cpp      # Barnes-Hut octree for efficient N-body force computation
    ├── Output.h/cpp      # CSV output handler for particle data
    ├── GeometryOutput.h/cpp  # CSV output handler for geometric objects (ROI, Shielding, Source)
    ├── utils.h           # Shared utilities: vector3<T>, uniform01(), distance()
    ├── test.cpp          # Unit tests using the doctest framework
    ├── output/           # Simulation output files (CSV) – generated at runtime
    └── dependencies/     # Third-party headers (place doctest.h here before testing)
```

---

## Prerequisites

| Requirement | Version | Notes |
|---|---|---|
| g++ | ≥ 9 | C++17 support required |
| make | any | Standard GNU make |
| doctest | v2.5.1 | Required **only** for running tests (see [Running the Tests](#running-the-tests)) |
| Doxygen | any | Required **only** for generating documentation |

---

## Running the Simulation

From the **repository root**, build and run the simulation with a single command:

```bash
make run
```

Alternatively, build first and then execute the binary manually:

```bash
make          # compiles to src/shielding_sim
cd src
./shielding_sim
```

Output CSV files are written to `src/output/`.

---

## Running the Tests

### 1. Install doctest

The test suite depends on the **doctest** single-header library (v2.5.1).  
Place `doctest.h` inside `src/dependencies/` using one of the following methods:

**Option A – curl (recommended):**
```bash
curl -LO --output-dir src/dependencies \
  https://raw.githubusercontent.com/doctest/doctest/refs/heads/master/doctest/doctest.h
```

**Option B – manual download:**  
Download [`doctest.h`](https://github.com/doctest/doctest/blob/master/doctest/doctest.h) from the official repository and place it in `src/dependencies/doctest.h`.

### 2. Run the tests

From the **repository root**:

```bash
make test
```

This target:
1. Cleans any previous build artifacts.
2. Recompiles the main simulation (so all object files are fresh).
3. Compiles and links the test binary (`src/runTests`).
4. Executes the test binary and reports results.

---

## Makefile Reference

All commands below can be run from the **repository root**.

| Command | Description |
|---|---|
| `make` | Compiles all source files and produces the `src/shielding_sim` executable. |
| `make run` | Compiles (if necessary) and immediately runs `src/shielding_sim`. |
| `make test` | Cleans, rebuilds, compiles the test binary (`src/runTests`), and runs all unit tests. Requires `src/dependencies/doctest.h`. |
| `make clean` | Removes all compiled object files and executables (`*.o`, `shielding_sim`, `runTests`). |
| `make rebuild` | Equivalent to `make clean` followed by `make` — performs a full recompilation from scratch. |
| `make gen-doc` | Generates HTML API documentation in `doc/html/` using Doxygen. Requires Doxygen to be installed. |

---

## ParaView Visualization

The simulation outputs two types of files that can be loaded into [ParaView](https://www.paraview.org/) for interactive 3-D visualization:

| File | Location | Format | Contents |
|---|---|---|---|
| `shielding.vtp` | `src/` | VTK PolyData | 3-D box mesh of the shielding volume |
| `roi.vtp` | `src/` | VTK PolyData | 3-D box mesh of the Region of Interest |
| `source.vtp` | `src/` | VTK PolyData | 3-D box mesh representing the radioactive source |
| `particles_<step>.csv` | `src/output/` | CSV | Particle positions (`x`, `y`, `z`), status (`alive`), and `type` at each recorded timestep |

### Loading the files in ParaView

1. **Open the geometry files** – use *File → Open* to load `shielding.vtp`, `roi.vtp`, and `source.vtp`. Click **Apply** after each to display them.
2. **Load particle data** – open any `particles_<step>.csv` file. After clicking **Apply**, apply a **TableToPoints** filter (*Filters → Alphabetical → TableToPoints*) and set the X, Y, Z columns to `x`, `y`, `z` respectively. Click **Apply** to display the particle cloud.
3. **Colour by particle type** – in the *Coloring* section of the Properties panel, select `type` to distinguish particle types by colour.
4. **Animate over time** – load multiple `particles_<step>.csv` snapshots and use ParaView's animation controls to step through the simulation timesteps.

![ParaView screenshot showing shielding, ROI, source geometry, and particle cloud coloured by type](https://github.com/user-attachments/assets/1ad7ed3e-229b-4f7a-8aa1-ee6ac3c76506)

---

## API Documentation

Pre-generated documentation is available by opening `doc/html/index.html` in any web browser.

To regenerate the documentation from source (requires [Doxygen](https://www.doxygen.nl/)):

```bash
make gen-doc
```

---

## Bug Reporting

If you encounter a bug, please [open a new issue](../../issues/new/choose) and select the **Bug Report** template.

Fill in the following fields:

| Field | Description |
|---|---|
| **Title** | A short, descriptive title for the issue. The title will be automatically formatted by the CI workflow. |
| **Severity** | Choose one of: 🔥 Blocker, 🛑 Major, ⚠️ Minor, ❓ Not sure. |
| **Steps to Reproduce & What Happens** | A numbered list of every step needed to trigger the bug, followed by a description of the observed (incorrect) behaviour. |
| **Expected Behavior** | A clear description of what should have happened instead. |

> **Note:** Labels are applied automatically – you do not need to add them manually.