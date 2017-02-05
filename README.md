## Lattice Implementation for a `beemaster` Alert Correlation Unit

#### Build

- git-checkout this repository
- on the same level in the folder tree, git-checkout the ACU-FW repository
- `make` the framework
- `make` this concrete implementation

#### Container build

The framework is included as submodule, so the container build is a lot easier. Simply do `docker build . -t acu && docker run acu`
