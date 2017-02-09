/* main.cc
 * ACU Implementation
 *
 * The entrypoint for this ACU.
 * Sets everything up and starts the work.
 *
 * @author: 1jost
 */

#include <acu/acu.h>
#include "lattice_alert_mapper.h"
#include "config_parser.h"
#include "rocks_storage.h"
#include "vector_storage.h"
#include "lattice_correlation.h"
#include "lattice_aggregation.h"
#include <csignal>
#include <unistd.h>

using namespace beemaster;

/// http://en.cppreference.com/w/cpp/utility/program/signal
void signal_handler(int signum) {
    // TODO do we want to keep this?
    std::cout << "Received signal " << signum << ". Aborting." << std::endl;
    exit(signum);
}

/// The main function to invoke this ACU
///
/// This program expects a path to the configuration to use and
/// fails, if none is given.
///
/// The configuration file uses basic INI format. Sections are
/// marked in brackets (`[section]`). Key-value pairs are separated
/// by an equals sign (`=`). Comments are introduced with a hash
/// (`#`).
///
/// The following entries are expected (case-sensitive!):
///     storage/location = <path of storage location>
///     receiver/address = <address to listen on>
///     receiver/port    = <port to listen on>
///     sender/address   = <address to send to>
///     sender/port      = <port to send to>
/// There are no default values (for now).
///
/// @param argc     Number of passed arguments
/// @param argv     Array of arguments
/// @return         Status
int main(int argc, char *argv[]) {
    // setup signal handler for endless loop
    std::signal(SIGINT, signal_handler);
    std::signal(SIGTERM, signal_handler);

    // parsing argument // alternatively: getopt(3)
    if (argc < 2) {
        throw std::runtime_error("Expect one argument for the configuration path!");
    }
    std::string config_file(argv[1]);

    // parsing config
    auto config = ConfigParser(config_file);
    // TODO provide defaults (maybe via default in Get...)
    auto rocks_path = config.GetString("storage", "location");
    auto r_address = config.GetString("receiver", "address");
    auto r_port = (acu::port_t) config.GetInt("receiver", "port");
    auto s_address = config.GetString("sender", "address");
    auto s_port = (acu::port_t) config.GetInt("sender", "port");

    // setup storages
    auto public_storage = new VectorStorage("MainStorage");
    auto lattice_storage = new RocksStorage<int>(rocks_path);

    // get mapper
    auto alert_mapper = new LatticeAlertMapper();

    // setup algorithms
    auto thresholds = new std::vector<beemaster::LatticeThreshold>;
    thresholds->push_back(beemaster::LatticeThreshold(0.7, "", ""));
    auto lattice = new LatticeCorrelation(public_storage, lattice_storage, thresholds, "beemaster/bro/lattice");

    // setup acu
    auto acu = acu::Acu(public_storage, alert_mapper);
    // - set connection details
    acu.SetReceiverInfo(r_address, r_port);
    acu.SetSenderInfo(s_address, s_port);
    // - add algorithms
    auto aggregationThresholds = new std::vector<acu::Threshold>;
    aggregationThresholds->push_back(acu::Threshold(210, "", ""));
    auto aggregation = new beemaster::LatticeAggregation(nullptr, aggregationThresholds);
    acu.Register(new std::vector<std::string>{"beemaster/bro/lattice"}, aggregation, lattice);

    // start acu
    acu.Run();
    for (;;) {
        acu.CheckForAlerts();
        usleep(100);
    }
    return 0;
}
