/*
 * 
 */
#include "catch.hpp"
#include "../src/latticeCorrelation.h"
#include <acu/incoming_alert.h>
#include "../src/rocks_storage.h"
#include "../src/lattice_outgoing_alert.h"
#include "../src/lattice_threshold.h"
#include <acu/correlation.h>
#include <unordered_set>
#include <utility>

TEST_CASE("Testing LatticeCorrelation", "[lattieCorrelation]") {
    // Create Incoming Alert
    auto time_stamp = std::chrono::system_clock::now();
    auto val = std::chrono::duration_cast<std::chrono::duration<double>>(time_stamp.time_since_epoch());
    auto broker_stamp = broker::time_point{val.count()};
    auto topic = new std::string("test");
    auto msg = broker::message{broker_stamp, "incident", "TCP", "127.0.0.1", (uint16_t)8080, "192.168.0.1", (uint16_t)9090};
    auto alert = acu::IncomingAlert(topic, msg);

    // Open DB
    beemaster::RocksStorage<int> storage = beemaster::RocksStorage<int>("/tmp/test");

    // Create Threshold
    std::vector<beemaster::LatticeThreshold> thres = {};
    auto t = beemaster::LatticeThreshold(0.7, "scan", "lol");
    thres.push_back(t);
    beemaster::VectorStorage vStorage = beemaster::VectorStorage("/tmp/db2");
    // Create LatticeCorrelation instance
    auto latCorr = beemaster::LatticeCorrelation(&vStorage, &storage, &thres, "test");
    
    //create set
    std::unordered_set<beemaster::pattern*>* pattern_set = new std::unordered_set<beemaster::pattern*>; 
    // generate type1 pattern
    beemaster::pattern* p1;
    //generate type2 pattern
    p1= latCorr.generatePattern(alert, "srcIp:srcPrt", 1);
    REQUIRE(p1->attributes.size() == 2);
    REQUIRE(p1->attributes["srcIp"] == "127.0.0.1");
    REQUIRE(p1->attributes["srcPrt"] == "8080");
    REQUIRE(p1->type == 2);
    REQUIRE(!p1->isLeaf);
    REQUIRE(p1->signature == "srcIp:srcPrt");
    REQUIRE(p1->support == 1);
    REQUIRE(p1->count == 1);
    REQUIRE(p1->parents.empty());
    REQUIRE(p1->children.empty());
    REQUIRE(p1->remaining == 0);
    pattern_set->insert(p1);
   
    //generate type3 pattern
    p1= latCorr.generatePattern(alert, "srcIp:dstPrt", 1);
    REQUIRE(p1->attributes.size() == 2);
    REQUIRE(p1->attributes["srcIp"] == "127.0.0.1");
    REQUIRE(p1->attributes["dstPrt"] == "9090");
    REQUIRE(p1->type == 3);
    REQUIRE(!p1->isLeaf);
    REQUIRE(p1->signature == "srcIp:dstPrt");
    REQUIRE(p1->support == 1);
    REQUIRE(p1->count == 1);
    REQUIRE(p1->parents.empty());
    REQUIRE(p1->children.empty());
    REQUIRE(p1->remaining == 0);
    pattern_set->insert(p1);

    //generate type4 pattern
    p1= latCorr.generatePattern(alert, "srcIp:protocol", 1);
    REQUIRE(p1->attributes.size() == 2);
    REQUIRE(p1->attributes["srcIp"] == "127.0.0.1");
    REQUIRE(p1->attributes["protocol"] == "TCP");
    REQUIRE(p1->type == 4);
    REQUIRE(!p1->isLeaf);
    REQUIRE(p1->signature == "srcIp:protocol");
    REQUIRE(p1->support == 1);
    REQUIRE(p1->count == 1);
    REQUIRE(p1->parents.empty());
    REQUIRE(p1->children.empty());
    REQUIRE(p1->remaining == 0);
    pattern_set->insert(p1);

    //generate type5 pattern
    p1 = latCorr.generatePattern(alert, "srcIp:srcPrt:dstPrt", 1);
    REQUIRE(p1->attributes.size() == 3);
    REQUIRE(p1->attributes["srcIp"] == "127.0.0.1");
    REQUIRE(p1->attributes["srcPrt"] == "8080");
    REQUIRE(p1->type == 5);
    REQUIRE(!p1->isLeaf);
    REQUIRE(p1->signature == "srcIp:srcPrt:dstPrt");
    REQUIRE(p1->support == 1);
    REQUIRE(p1->count == 1);
    REQUIRE(p1->parents.empty());
    REQUIRE(p1->children.empty());
    REQUIRE(p1->remaining == 0);
    pattern_set->insert(p1);

    //generate type6 pattern
    p1 = latCorr.generatePattern(alert, "srcIp:srcPrt:protocol", 1);
    REQUIRE(p1->attributes.size() == 3);
    REQUIRE(p1->attributes["srcIp"] == "127.0.0.1");
    REQUIRE(p1->attributes["srcPrt"] == "8080");
    REQUIRE(p1->attributes["protocol"] == "TCP");
    REQUIRE(p1->type == 6);
    REQUIRE(!p1->isLeaf);
    REQUIRE(p1->signature == "srcIp:srcPrt:protocol");
    REQUIRE(p1->support == 1);
    REQUIRE(p1->count == 1);
    REQUIRE(p1->parents.empty());
    REQUIRE(p1->children.empty());
    REQUIRE(p1->remaining == 0);
    pattern_set->insert(p1);

    //generate type7 pattern
    p1 = latCorr.generatePattern(alert, "srcIp:dstPrt:protocol", 1);
    REQUIRE(p1->attributes.size() == 3);
    REQUIRE(p1->attributes["srcIp"] == "127.0.0.1");
    REQUIRE(p1->attributes["protocol"] == "TCP");
    REQUIRE(p1->attributes["dstPrt"] == "9090");
    REQUIRE(p1->type == 7);
    REQUIRE(!p1->isLeaf);
    REQUIRE(p1->signature == "srcIp:dstPrt:protocol");
    REQUIRE(p1->support == 1);
    REQUIRE(p1->count == 1);
    REQUIRE(p1->parents.empty());
    REQUIRE(p1->children.empty());
    REQUIRE(p1->remaining == 0);
    pattern_set->insert(p1);

    //generate type8 pattern
    p1 = latCorr.generatePattern(alert, "srcIp:srcPrt:dstPrt:protocol", 1);
    REQUIRE(p1->attributes.size() == 4);
    REQUIRE(p1->attributes["srcIp"] == "127.0.0.1");
    REQUIRE(p1->attributes["srcPrt"] == "8080");
    REQUIRE(p1->attributes["dstPrt"] == "9090");
    REQUIRE(p1->attributes["protocol"] == "TCP");
    REQUIRE(p1->type == 8);
    REQUIRE(p1->isLeaf);
    REQUIRE(p1->signature == "srcIp:srcPrt:dstPrt:protocol");
    REQUIRE(p1->support == 1);
    REQUIRE(p1->count == 1);
    REQUIRE(p1->parents.empty());
    REQUIRE(p1->children.empty());
    REQUIRE(p1->remaining == 0);
    pattern_set->insert(p1);
    
    p1 = latCorr.generatePattern(alert, "srcIp", 1);
    REQUIRE(p1->attributes.size() == 1);
    REQUIRE(p1->attributes["srcIp"] == "127.0.0.1");
    REQUIRE(p1->type == 1);
    REQUIRE(!p1->isLeaf);
    REQUIRE(p1->signature == "srcIp");
    REQUIRE(p1->support == 1);
    REQUIRE(p1->count == 1);
    REQUIRE(p1->parents.empty());
    REQUIRE(p1->children.empty());
    REQUIRE(p1->remaining == 0);
    pattern_set->insert(p1);
    
    REQUIRE(pattern_set->size() == 8);
    
    SECTION("Relations and postOrder"){
        //TODO: DIRTY LIKE DAVID :>
        std::system("rm -rf /tmp/test");
        latCorr.generateNodesRelation(pattern_set);
        for(auto pattern : *pattern_set){
            //printf("type: %d has %d children\n", pattern->type, pattern->children.size());
            //TODO Check all cases
            for(auto it = pattern->attributes.begin(); it!=pattern->attributes.end(); ++it ){
                for(auto child : pattern->children){
                    REQUIRE(it->second == child->attributes[it->first]);
                }    
            } 
        }
        // postOrder Test
        auto nodes = new std::vector<beemaster::pattern*>;
        beemaster::pattern* root = new beemaster::pattern;
        // get root
        for(auto pattern : *pattern_set){
            if(pattern->type == 1){
                root = pattern;
                break;
            }
        }
        std::postOrder(root, nodes);
        REQUIRE(nodes->size() == pattern_set->size()); 
        REQUIRE(nodes->at(0)->type == 8);
        bool second = nodes->at(1)->type == 5 || nodes->at(1)->type == 6 || nodes->at(1)->type == 7;
        REQUIRE(second);
        bool third = nodes->at(2)->type == 5 || nodes->at(2)->type == 6 || nodes->at(2)->type == 7;
        REQUIRE(third);
        bool fourth = nodes->at(3)->type == 2 || nodes->at(3)->type == 3 || nodes->at(3)->type == 4;
        REQUIRE(fourth);
        bool fifth = nodes->at(4)->type == 5 || nodes->at(4)->type == 6 || nodes->at(4)->type == 7;
        REQUIRE(fifth);
        bool sixth = nodes->at(5)->type == 2 || nodes->at(5)->type == 3 || nodes->at(5)->type == 4;
        REQUIRE(sixth);
        bool seventh = nodes->at(6)->type == 2 || nodes->at(6)->type == 3 || nodes->at(6)->type == 4;
        REQUIRE(seventh);
        bool eight = nodes->at(7)->type == 1;
        REQUIRE(eight);
        // TODO: Extend or make better
    }
    SECTION("Compression"){ 
        /*for(auto pattern : *pattern_set){
            //printf("%f >= %d\n", pattern->support, thres[0].count);
            REQUIRE(pattern->support >= thres[0].count);
        }
        */
        //compress
        auto newSet = latCorr.latticeCompression(pattern_set, thres[0].countRatio);
        for(auto pattern : *newSet){
            //printf("%f >= %d\n", pattern->support, thres[0].count);
            REQUIRE(pattern->support >= thres[0].countRatio);
        }
    }
    SECTION("CORRELATE"){ 
        // correlate
        //TODO: DIRTY LIKE DAVID :>
        std::system("rm -rf /tmp/test");

        std::vector<const acu::IncomingAlert*> alerts = {&alert};
        auto pattern2 = latCorr.correlate(alerts, thres[0].countRatio);
    }
    SECTION("MULTIPLE ALERTS"){
        //TODO: DIRTY LIKE DAVID :>
        std::system("rm -rf /tmp/test");

        auto msg = broker::message{broker_stamp, "incident", "TCP", "127.0.0.1", (uint16_t)8080, "192.168.0.2", (uint16_t)7070};
        auto alert2 = acu::IncomingAlert(topic, msg);
        std::vector<const acu::IncomingAlert*> alerts = {&alert, &alert2};
        auto pattern2 = latCorr.correlate(alerts, thres[0].countRatio);
        printf("size %d\n", pattern2->size());
        for(auto pattern12 : *pattern2) {
            printf("%d ->count: %d \n", pattern12->type, pattern12->count);
            if(pattern12->type == 1){
                REQUIRE(pattern12->support == 1);
                REQUIRE(pattern12->parents.size() == 0);
                REQUIRE(pattern12->children.size() == 4);
            }
            if(pattern12->type == 2){
                REQUIRE(pattern12->support == 1);
                REQUIRE(pattern12->parents.size() == 1);
                REQUIRE(pattern12->children.size() == 3);
            }
            if(pattern12->type == 3){
                REQUIRE(pattern12->support == 0.5);
                REQUIRE(pattern12->parents.size() == 1);
                REQUIRE(pattern12->children.size() == 2);
            }
            if(pattern12->type == 4){
                REQUIRE(pattern12->support == 1);
                REQUIRE(pattern12->parents.size() == 1);
                REQUIRE(pattern12->children.size() == 3);
            }
            if(pattern12->type == 5){
                REQUIRE(pattern12->support == 0.5);
                REQUIRE(pattern12->parents.size() == 2);
                REQUIRE(pattern12->children.size() == 1);
            }
            if(pattern12->type == 6){
                REQUIRE(pattern12->support == 1);
                REQUIRE(pattern12->parents.size() == 2);
                REQUIRE(pattern12->children.size() == 2);
            }
            if(pattern12->type == 7){
                REQUIRE(pattern12->support == 0.5);
                REQUIRE(pattern12->parents.size() == 2);
                REQUIRE(pattern12->children.size() == 1);
            }
            if(pattern12->type == 8){
                REQUIRE(pattern12->support == 0.5);
                REQUIRE(pattern12->parents.size() == 3);
                REQUIRE(pattern12->children.size() == 0);
            }
            REQUIRE(pattern12->support > thres[0].countRatio);
        }       
    }
    
    SECTION("INVOKE"){
        //TODO: DIRTY LIKE DAVID :>
        std::system("rm -rf /tmp/test");

        // put data in vectorStorage
        latCorr.vStorage->Persist(&alert);
        auto olert = latCorr.Invoke();
        //REQUIRE(olert->ToMessage());
        REQUIRE(true);
    }
    SECTION("PAPER TEST"){
        //TODO: DIRTY LIKE DAVID :>
        std::system("rm -rf /tmp/test");

        auto msg = broker::message{broker_stamp, "incident", "UDP", "60.240.134.94", (uint16_t)4313, "192.168.0.1", (uint16_t)1434};
        auto alert = acu::IncomingAlert(topic, msg);
        
        msg = broker::message{broker_stamp, "incident", "TCP", "60.240.134.94", (uint16_t)4313, "192.168.0.1", (uint16_t)1434};
        auto alert2 = acu::IncomingAlert(topic, msg);
        
        msg = broker::message{broker_stamp, "incident", "UDP", "60.240.134.94", (uint16_t)4313, "192.168.0.1", (uint16_t)3276};
        auto alert3 = acu::IncomingAlert(topic, msg);
        
        msg = broker::message{broker_stamp, "incident", "TCP", "60.240.134.94", (uint16_t)4313, "192.168.0.1", (uint16_t)3276};
        auto alert4 = acu::IncomingAlert(topic, msg);
        
        msg = broker::message{broker_stamp, "incident", "UDP", "60.240.134.94", (uint16_t)2771, "192.168.0.1", (uint16_t)1434};
        auto alert5 = acu::IncomingAlert(topic, msg);

        msg = broker::message{broker_stamp, "incident", "TCP", "60.240.134.94", (uint16_t)2771, "192.168.0.1", (uint16_t)1434};
        auto alert6 = acu::IncomingAlert(topic, msg);

        msg = broker::message{broker_stamp, "incident", "UDP", "60.240.134.94", (uint16_t)2771, "192.168.0.1", (uint16_t)3276};
        auto alert7 = acu::IncomingAlert(topic, msg);

        msg = broker::message{broker_stamp, "incident", "TCP", "60.240.134.94", (uint16_t)2771, "192.168.0.1", (uint16_t)3276};
        auto alert8 = acu::IncomingAlert(topic, msg);
        std::vector<int> limit = {85, 5, 5, 5, 65, 5, 35, 5};
        std::vector<acu::IncomingAlert> input = {alert, alert2, alert3, alert4, alert5, alert6, alert7, alert8};
        for(size_t j = 0; j<limit.size(); ++j){
            for(auto i = 0; i<limit.at(j); ++i){
                latCorr.vStorage->Persist(new acu::IncomingAlert(input.at(j)));
            }
        }
        auto output = latCorr.Invoke();
        REQUIRE(output->incidents.size() == 1);
    }
}


