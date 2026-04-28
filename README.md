# Radioactive Shielding Simulation

A 3-D radioactive particle transport simulator developed for **COSC 1560 – Computer Programming II**.  
The program models the emission of particles from a radioactive point source, their propagation through a configurable shielding volume, and their scoring inside a user-defined Region of Interest (ROI).  
Coulomb interactions between particles are approximated with the [Barnes-Hut algorithm](https://en.wikipedia.org/wiki/Barnes%E2%80%93Hut_simulation) via an octree spatial index.

---

## Table of Contents

1. [Project Description](#project-description)
2. [Project File Structure](#project-file-structure)
3. [Prerequisites](#prerequisites)
4. [Running the Simulation](#running-the-simulation)
5. [Running the Tests](#running-the-tests)
6. [Makefile Reference](#makefile-reference)
7. [API Documentation](#api-documentation)
8. [Bug Reporting](#bug-reporting)

---

## Project Description

The simulation proceeds in discrete time steps:

1. The **Source** emits new particles each step.
2. Every live particle is moved according to its velocity and the electrostatic forces computed by the **Octree** (Barnes-Hut).
3. Particles that enter the **Shielding** volume are attenuated or absorbed depending on the chosen material.
4. Particles inside the **ROI** are scored and periodically written to CSV files in `src/output/`.

---

## Project File Structure

```
radioactiveSim/
├── Doxyfile              # Doxygen configuration for API documentation generation
├── Makefile              # Root-level wrapper that forwards targets to src/makefile
├── README.md
├── doc/
│   └── html/             # Generated HTML documentation (produced by `make gen-doc`)
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