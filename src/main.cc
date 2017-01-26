/* main.cc
 * Logging ACU Implementation
 * Simply logs everything that is received via broker
 */

#include <acu/acu.h>
#include "config_parser.h"
#include "rocks_storage.h"

#include "batch_aggregation.h"
#include "batch_logging_correlation.h"

#include <iostream>
#include <csignal>
#include <unistd.h>

using namespace beemaster;

void signal_handler(int signum) {
    // TODO do we want to keep this?
    std::cout << "Received signal " << signum << ". Aborting." << std::endl;
    exit(signum);
}

/// The main function to invoke this batch-logging ACU
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
int main(int argc, char* argv[]) {
    // setup signal handler for endless loop
    std::signal(SIGINT, signal_handler);
    std::signal(SIGTERM, signal_handler);

    if (argc < 2)
        throw std::runtime_error("Expect one argument for the configuration path!");
    std::string config_file(argv[1]);

    // parsing config
    auto config = ConfigParser(config_file);

    auto rocks_path = config.GetString("storage", "location");
    auto rec_addr = config.GetString("receiver", "address");
    auto rec_port = (acu::port_t)config.GetInt("receiver", "port");
    auto send_addr = config.GetString("sender", "address");
    auto send_port = (acu::port_t)config.GetInt("sender", "port");

    // setup storage
    auto storage = new RocksStorage<int>(rocks_path);

    // get mapper
    auto alert_mapper = new acu::AlertMapper();

    // setup algorithms
    auto batch_thr = new std::vector<acu::Threshold>;
    batch_thr->push_back(acu::Threshold(5, "", ""));

    auto corr_thr = new std::vector<acu::Threshold>;
    batch_thr->push_back(acu::Threshold(10, "", ""));

    auto batcher = new BatchAggregation(storage, batch_thr);
    auto logger = new BatchLoggingCorrelation(storage, corr_thr);

    auto acu = acu::Acu(storage, alert_mapper);
    acu.SetReceiverInfo(rec_addr, rec_port);
    acu.SetSenderInfo(send_addr, send_port);

    acu.Register(new std::vector<std::string>{"test/topic"}, batcher, logger);

    // start acu
    acu.Run();
    for (;;) {
        //acu.CheckForAlerts();
        usleep(10); // without this you burn one core of your CPU
    }

    return 0;
}