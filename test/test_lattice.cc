/*
 * 
 */
#include "catch.hpp"
#include "../src/latticeCorrelation.h"
#include <acu/incoming_alert.h>
#include "../src/rocks_storage.h"
#include <acu/correlation.h>

TEST_CASE("Testing Pattern Generation", "[lattieCorrelation]") {
    // Create Incoming Alert
    auto time_stamp = std::chrono::system_clock::now();
    auto val = std::chrono::duration_cast<std::chrono::duration<double>>(time_stamp.time_since_epoch());
    auto broker_stamp = broker::time_point{val.count()};
    auto topic = new std::string("some topic");
    auto msg = broker::message{broker_stamp, "incident", "TCP", "127.0.0.1", (uint16_t)8080, "192.168.0.1", (uint16_t)9090};
    auto alert = acu::IncomingAlert(topic, msg);

    // Open DB
    auto db = beemaster::RocksStorage("/tmp/test");

    // Create Threshold
    auto thres = {acu::Threshold(0, "scan", "lol")};
    
    // Create LatticeCorrelation instance
    auto latCorr = beemaster::LatticeCorrelation(&db, &thres);
}
