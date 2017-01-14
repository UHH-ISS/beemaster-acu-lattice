/*
 * 
 */
#include "catch.hpp"
#include "../src/latticeCorrelation.h"
#include <acu/incoming_alert.h>
#include "../src/rocks_storage.h"
#include <acu/correlation.h>
#include <unordered_set>

TEST_CASE("Testing LatticeCorrelation", "[lattieCorrelation]") {
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
    
    //create set
    std::unordered_set<beemaster::pattern*> pattern_set = {}; 
    // generate type1 pattern
    beemaster::pattern* p1 = latCorr.generatePattern(alert, "srcIp", 1);
    REQUIRE(p1->srcIp == "127.0.0.1");
    REQUIRE(p1->srcPrt == 0);
    REQUIRE(p1->dstPrt == 0);
    REQUIRE(p1->protocol == "");
    REQUIRE(p1->type == 1);
    REQUIRE(!p1->isLeaf);
    REQUIRE(p1->signature == "srcIp");
    REQUIRE(p1->support == 1);
    REQUIRE(p1->count == 1);
    REQUIRE(p1->parents.empty());
    REQUIRE(p1->children.empty());
    REQUIRE(p1->remaining == 0);
    pattern_set.insert(p1);

    //generate type2 pattern
    p1= latCorr.generatePattern(alert, "srcIp:srcPrt", 1);
    REQUIRE(p1->srcIp == "127.0.0.1");
    REQUIRE(p1->srcPrt == 8080);
    REQUIRE(p1->dstPrt == 0);
    REQUIRE(p1->protocol == "");
    REQUIRE(p1->type == 2);
    REQUIRE(!p1->isLeaf);
    REQUIRE(p1->signature == "srcIp:srcPrt");
    REQUIRE(p1->support == 1);
    REQUIRE(p1->count == 1);
    REQUIRE(p1->parents.empty());
    REQUIRE(p1->children.empty());
    REQUIRE(p1->remaining == 0);
    pattern_set.insert(p1);
   
    //generate type3 pattern
    p1= latCorr.generatePattern(alert, "srcIp:dstPrt", 1);
    REQUIRE(p1->srcIp == "127.0.0.1");
    REQUIRE(p1->srcPrt == 0);
    REQUIRE(p1->dstPrt == 9090);
    REQUIRE(p1->protocol == "");
    REQUIRE(p1->type == 3);
    REQUIRE(!p1->isLeaf);
    REQUIRE(p1->signature == "srcIp:dstPrt");
    REQUIRE(p1->support == 1);
    REQUIRE(p1->count == 1);
    REQUIRE(p1->parents.empty());
    REQUIRE(p1->children.empty());
    REQUIRE(p1->remaining == 0);
    pattern_set.insert(p1);

    //generate type4 pattern
    p1= latCorr.generatePattern(alert, "srcIp:protocol", 1);
    REQUIRE(p1->srcIp == "127.0.0.1");
    REQUIRE(p1->srcPrt == 0);
    REQUIRE(p1->dstPrt == 0);
    REQUIRE(p1->protocol == "TCP");
    REQUIRE(p1->type == 4);
    REQUIRE(!p1->isLeaf);
    REQUIRE(p1->signature == "srcIp:protocol");
    REQUIRE(p1->support == 1);
    REQUIRE(p1->count == 1);
    REQUIRE(p1->parents.empty());
    REQUIRE(p1->children.empty());
    REQUIRE(p1->remaining == 0);
    pattern_set.insert(p1);

    //generate type5 pattern
    p1 = latCorr.generatePattern(alert, "srcIp:srcPrt:dstPrt", 1);
    REQUIRE(p1->srcIp == "127.0.0.1");
    REQUIRE(p1->srcPrt == 8080);
    REQUIRE(p1->dstPrt == 9090);
    REQUIRE(p1->protocol == "");
    REQUIRE(p1->type == 5);
    REQUIRE(!p1->isLeaf);
    REQUIRE(p1->signature == "srcIp:srcPrt:dstPrt");
    REQUIRE(p1->support == 1);
    REQUIRE(p1->count == 1);
    REQUIRE(p1->parents.empty());
    REQUIRE(p1->children.empty());
    REQUIRE(p1->remaining == 0);
    pattern_set.insert(p1);

    //generate type6 pattern
    p1 = latCorr.generatePattern(alert, "srcIp:srcPrt:protocol", 1);
    REQUIRE(p1->srcIp == "127.0.0.1");
    REQUIRE(p1->srcPrt == 8080);
    REQUIRE(p1->dstPrt == 0);
    REQUIRE(p1->protocol == "TCP");
    REQUIRE(p1->type == 6);
    REQUIRE(!p1->isLeaf);
    REQUIRE(p1->signature == "srcIp:srcPrt:protocol");
    REQUIRE(p1->support == 1);
    REQUIRE(p1->count == 1);
    REQUIRE(p1->parents.empty());
    REQUIRE(p1->children.empty());
    REQUIRE(p1->remaining == 0);
    pattern_set.insert(p1);

    //generate type7 pattern
    p1 = latCorr.generatePattern(alert, "srcIp:dstPrt:protocol", 1);
    REQUIRE(p1->srcIp == "127.0.0.1");
    REQUIRE(p1->srcPrt == 0);
    REQUIRE(p1->dstPrt == 9090);
    REQUIRE(p1->protocol == "TCP");
    REQUIRE(p1->type == 7);
    REQUIRE(!p1->isLeaf);
    REQUIRE(p1->signature == "srcIp:dstPrt:protocol");
    REQUIRE(p1->support == 1);
    REQUIRE(p1->count == 1);
    REQUIRE(p1->parents.empty());
    REQUIRE(p1->children.empty());
    REQUIRE(p1->remaining == 0);
    pattern_set.insert(p1);

    //generate type8 pattern
    p1 = latCorr.generatePattern(alert, "srcIp:srcPrt:dstPrt:protocol", 1);
    REQUIRE(p1->srcIp == "127.0.0.1");
    REQUIRE(p1->srcPrt == 8080);
    REQUIRE(p1->dstPrt == 9090);
    REQUIRE(p1->protocol == "TCP");
    REQUIRE(p1->type == 8);
    REQUIRE(p1->isLeaf);
    REQUIRE(p1->signature == "srcIp:srcPrt:dstPrt:protocol");
    REQUIRE(p1->support == 1);
    REQUIRE(p1->count == 1);
    REQUIRE(p1->parents.empty());
    REQUIRE(p1->children.empty());
    REQUIRE(p1->remaining == 0);
    pattern_set.insert(p1);
    REQUIRE(pattern_set.size() == 8);
    latCorr.generateNodesRelation(&pattern_set);
    for(auto pattern : pattern_set){
        //TODO Check all cases
        if(pattern->type == 1){        
            for(auto child: pattern->children){
                auto boel = child->type == 2 || child->type == 3 || child->type == 4;
                REQUIRE(boel);
                REQUIRE(pattern->srcIp == child->srcIp);
                REQUIRE(pattern->parents.size() == 0);
            }
        }
        if(pattern->type == 2){
            for(auto child: pattern->children){
                auto boel = child->type == 6 || child->type == 5;
                REQUIRE(boel);
                REQUIRE(pattern->srcIp == child->srcIp);
                REQUIRE(pattern->srcPrt == child->srcPrt);
            }
        }
        if(pattern->type == 3){
            for(auto child: pattern->children){
                auto boel = child->type == 7 || child->type == 5;
                REQUIRE(boel);
                REQUIRE(pattern->srcIp == child->srcIp);
                REQUIRE(pattern->dstPrt == child->dstPrt);
            }
        }
        if(pattern->type == 4){        
            for(auto child: pattern->children){
                auto boel = child->type == 7 || child->type == 6;
                REQUIRE(boel);
                REQUIRE(pattern->srcIp == child->srcIp);
                REQUIRE(pattern->protocol == child->protocol);
            }
        }
        if(pattern->type == 5){
            for(auto child: pattern->children){
                REQUIRE(child->type == 8);
                REQUIRE(pattern->srcIp == child->srcIp);
                REQUIRE(pattern->srcPrt == child->srcPrt);
                REQUIRE(pattern->dstPrt == child->dstPrt);
            }
        }
        if(pattern->type == 6){
            for(auto child: pattern->children){
                REQUIRE(child->type == 8);
                REQUIRE(pattern->srcIp == child->srcIp);
                REQUIRE(pattern->srcPrt == child->srcPrt);
                REQUIRE(pattern->protocol == child->protocol);
            }
        }
        if(pattern->type == 7){
            for(auto child: pattern->children){
                REQUIRE(child->type == 8);
                REQUIRE(pattern->srcIp == child->srcIp);
                REQUIRE(pattern->protocol == child->protocol);
                REQUIRE(pattern->dstPrt == child->dstPrt);
            }
        }
    }
    std::vector<beemaster::pattern*>* nodes = new std::vector<beemaster::pattern*>;
    beemaster::pattern* root;
    // get root
    for(auto pattern : pattern_set){
        if(pattern->type == 1){
            root = pattern;
            break;
        }
    }
    std::postOrder(*root, nodes);
    REQUIRE(nodes->size() == pattern_set.size());
    // TODO: MORE TESTS! 
    
    // correlate
    std::vector<acu::IncomingAlert> alerts = {alert};
}


