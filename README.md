# MP-IDS Alert Correlation Unit: Custom Beemaster ACU Lattice

Beemaster ships with two custom ACU implementations, using the Beemaster [ACU framework](https://github.com/UHH-ISS/beemaster-acu-fw). The `master` branch of this repository contains the general Beemaster setup for custom ACUs, featuring a build setup and Dockerfile. Additionally, it contains the sources for a portscan detection ACU.

## Build & Run

This repository references the ACU framework repository as a submodule in [beemaster-acu-fw](https://github.com/UHH-ISS/beemaster-acu-fw). Please use `git clone --recursive` when cloning this repo.


#### Manual Build & Run

A call to `make` will first build the framework and then build the actual ACU. This needs to be done, since the ACU links against the framework library `.so`. The compiled binary of the ACU can then be found in the `build/` directory.

The compiled binary inside the build directory can be executed. A config file must be provided as first argument. Use the following command for a local start (local means that a Bro master is running on the same host as the ACU): `build/src/acu-impl config-local.ini`

#### Container Build & Run

This repository contains a [Dockerfile](Dockerfile). It can be uesd to build the ACU with all required resources bundled into a Docker image (e.g. via `docker build . -t acu`).

The dockerized ACU can be run with `docker run acu`.

You can use the [start.sh](start.sh) script. It executes the two above commands.

## Lattice

The Lattice-ACU is configured with an aggregation threshold of 210 alerts. The aggregation just acts as a simple counter instance to prevent the correlation to be invoked at every `LatticeIncomingAlert`. There is no persistence on the hard drive - everything is stored at runtime. When the correlation is invoked the `LatticeCorrelation` generates pattern instances out of the alert data. Furthermore, it filters insignificant instances and compresses them afterwards. An `OutgoingAlert` is given as the correlation output. It returns the relevant pattern types. The pattern type corresponds to a specific attack.

**Implementation details:** The ACU-Framework to the requirements of the lattice algorithm we extended several base classes beside an own `Aggregation` and `Correlation`. To reflect the need of knowing the protocol `LatticeIncomingAlert` was created, which is built by the `LatticeAlertMapper`. Furthermore, we needed a float value for our correlation threshold. Therefore `LatticeThreshold` was implemented. It serves as a threshold to determine which pattern instances are significant enough to pass through. The `VectorStorage` holds all alerts in a vector inside a map with the topic as the key. The correlation can easily access the map by requesting the vector for a specific topic and the storage pops them accordingly.

The [Lattice ACU](https://github.com/UHH-ISS/beemaster-acu-lattice) may be found [here](https://github.com/UHH-ISS/beemaster-acu-lattice).
