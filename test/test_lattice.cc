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
    beemaster::RocksStorage db = beemaster::RocksStorage("/tmp/test");

    // Create Threshold
    std::vector<acu::Threshold> thres = {};
    auto t = acu::Threshold(0, "scan", "lol");
    thres.push_back(t);
    
    // Create LatticeCorrelation instance
    auto latCorr = beemaster::LatticeCorrelation(&db, &thres);
    
    // generate type1 pattern
    beemaster::pattern p1 = latCorr.generatePattern(alert, "srcIp", 1);
    
    REQUIRE(p1.srcIp == "127.0.0.1");
    REQUIRE(p1.srcPrt == 0);
    REQUIRE(p1.dstPrt == 0);
    REQUIRE(p1.protocol == "");
    REQUIRE(p1.type == 1);
    REQUIRE(!p1.isLeaf);
    REQUIRE(p1.signature == "srcIp");
    REQUIRE(p1.support == 1);
    REQUIRE(p1.count == 1);
    REQUIRE(p1.parents.empty());
    REQUIRE(p1.children.empty());
    REQUIRE(p1.remaining == 0);

    //generate type2 pattern
    p1 = latCorr.generatePattern(alert, "srcIp:srcPrt", 1);
    REQUIRE(p1.srcIp == "127.0.0.1");
    REQUIRE(p1.srcPrt == 8080);
    REQUIRE(p1.dstPrt == 0);
    REQUIRE(p1.protocol == "");
    REQUIRE(p1.type == 2);
    REQUIRE(!p1.isLeaf);
    REQUIRE(p1.signature == "srcIp:srcPrt");
    REQUIRE(p1.support == 1);
    REQUIRE(p1.count == 1);
    REQUIRE(p1.parents.empty());
    REQUIRE(p1.children.empty());
    REQUIRE(p1.remaining == 0);

    //generate type3 pattern
    p1 = latCorr.generatePattern(alert, "srcIp:dstPrt", 1);
    REQUIRE(p1.srcIp == "127.0.0.1");
    REQUIRE(p1.srcPrt == 0);
    REQUIRE(p1.dstPrt == 9090);
    REQUIRE(p1.protocol == "");
    REQUIRE(p1.type == 3);
    REQUIRE(!p1.isLeaf);
    REQUIRE(p1.signature == "srcIp:dstPrt");
    REQUIRE(p1.support == 1);
    REQUIRE(p1.count == 1);
    REQUIRE(p1.parents.empty());
    REQUIRE(p1.children.empty());
    REQUIRE(p1.remaining == 0);

    //generate type4 pattern
    p1 = latCorr.generatePattern(alert, "srcIp:protocol", 1);
    REQUIRE(p1.srcIp == "127.0.0.1");
    REQUIRE(p1.srcPrt == 0);
    REQUIRE(p1.dstPrt == 0);
    REQUIRE(p1.protocol == "TCP");
    REQUIRE(p1.type == 4);
    REQUIRE(!p1.isLeaf);
    REQUIRE(p1.signature == "srcIp:protocol");
    REQUIRE(p1.support == 1);
    REQUIRE(p1.count == 1);
    REQUIRE(p1.parents.empty());
    REQUIRE(p1.children.empty());
    REQUIRE(p1.remaining == 0);

    //generate type5 pattern
    p1 = latCorr.generatePattern(alert, "srcIp:srcPrt:dstPrt", 1);
    REQUIRE(p1.srcIp == "127.0.0.1");
    REQUIRE(p1.srcPrt == 8080);
    REQUIRE(p1.dstPrt == 9090);
    REQUIRE(p1.protocol == "");
    REQUIRE(p1.type == 5);
    REQUIRE(!p1.isLeaf);
    REQUIRE(p1.signature == "srcIp:srcPrt:dstPrt");
    REQUIRE(p1.support == 1);
    REQUIRE(p1.count == 1);
    REQUIRE(p1.parents.empty());
    REQUIRE(p1.children.empty());
    REQUIRE(p1.remaining == 0);

    //generate type6 pattern
    p1 = latCorr.generatePattern(alert, "srcIp:srcPrt:protocol", 1);
    REQUIRE(p1.srcIp == "127.0.0.1");
    REQUIRE(p1.srcPrt == 8080);
    REQUIRE(p1.dstPrt == 0);
    REQUIRE(p1.protocol == "TCP");
    REQUIRE(p1.type == 6);
    REQUIRE(!p1.isLeaf);
    REQUIRE(p1.signature == "srcIp:srcPrt:protocol");
    REQUIRE(p1.support == 1);
    REQUIRE(p1.count == 1);
    REQUIRE(p1.parents.empty());
    REQUIRE(p1.children.empty());
    REQUIRE(p1.remaining == 0);

    //generate type7 pattern
    p1 = latCorr.generatePattern(alert, "srcIp:dstPrt:protocol", 1);
    REQUIRE(p1.srcIp == "127.0.0.1");
    REQUIRE(p1.srcPrt == 0);
    REQUIRE(p1.dstPrt == 9090);
    REQUIRE(p1.protocol == "TCP");
    REQUIRE(p1.type == 7);
    REQUIRE(!p1.isLeaf);
    REQUIRE(p1.signature == "srcIp:dstPrt:protocol");
    REQUIRE(p1.support == 1);
    REQUIRE(p1.count == 1);
    REQUIRE(p1.parents.empty());
    REQUIRE(p1.children.empty());
    REQUIRE(p1.remaining == 0);

    //generate type8 pattern
    p1 = latCorr.generatePattern(alert, "srcIp:srcPrt:dstPrt:protocol", 1);
    REQUIRE(p1.srcIp == "127.0.0.1");
    REQUIRE(p1.srcPrt == 8080);
    REQUIRE(p1.dstPrt == 9090);
    REQUIRE(p1.protocol == "TCP");
    REQUIRE(p1.type == 8);
    REQUIRE(!p1.isLeaf);
    REQUIRE(p1.signature == "srcIp:srcPrt:dstPrt:protocol");
    REQUIRE(p1.support == 1);
    REQUIRE(p1.count == 1);
    REQUIRE(p1.parents.empty());
    REQUIRE(p1.children.empty());
    REQUIRE(p1.remaining == 0);
}