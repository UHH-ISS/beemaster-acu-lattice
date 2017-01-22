/* main.cc
 * ACU Implementation
 *
 * The entrypoint for this ACU.
 * Sets everything up and starts the work.
 *
 * @author: 1jost
 */

#include <acu/acu.h>
#include "rocks_storage.h"
#include "vector_storage.h"
#include "latticeCorrelation.h"
#include <iostream>
#include <csignal>

using namespace beemaster;

/// http://en.cppreference.com/w/cpp/utility/program/signal
void signal_handler(int signum) {
    // TODO do we want to keep this?
    std::cout << "Received signal " << signum << ". Aborting." << std::endl;
    exit(signum);
}

int main() {
    // setup signal handler for endless loop
    std::signal(SIGINT, signal_handler);
    std::signal(SIGTERM, signal_handler);

    // TODO EXTRACT AND CORRECT CONSTANTS

    // setup storages
    auto public_storage = new VectorStorage("MainStorage");
    auto lattice_storage = new RocksStorage<int>("/tmp/lattice_rocks");

    // get mapper
    auto alert_mapper = new acu::AlertMapper();

    // setup algorithms
    auto thresholds = new std::vector<acu::Threshold>;
    thresholds->push_back(acu::Threshold(1, "test", "whatever"));
    auto lattice = new LatticeCorrelation(lattice_storage, thresholds);

    // setup acu
    // TODO set connection details (currently not exposed in fw)
    auto acu = acu::Acu(public_storage, alert_mapper);
    // - add algorithms
    acu.Register(new std::vector<std::string>{"/connection"}, nullptr, lattice);

    // start acu
    acu.Run();
    for (;;)
        acu.CheckForAlerts();

    return 0;
}
