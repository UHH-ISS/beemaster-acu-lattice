//
// Created by florian on 12/1/16.
//
#include "latticeCorrelation.h"
#include <acu/correlation.h>
#include <acu/storage.h>
#include <acu/outgoing_alert.h>
#include <acu/incoming_alert.h>
#include "rocks_storage.h"
#include <acu/threshold.h>
#include <unordered_set>
#include <unordered_map>
#include <string>
#include <iostream>
#include <array>
#include <vector>
#include <iterator>
#include <algorithm>
#include <chrono>
using namespace std;
// hash func for unordered_set
namespace std{
    // Merge any unordered_set<pattern> instances
    unordered_set<beemaster::pattern> merge_set(const std::vector<unordered_set<beemaster::pattern>>& p){
        unordered_set<beemaster::pattern> setUnion = {};
        for(auto pattern_set : p){
            for(beemaster::pattern p1 : pattern_set){
                setUnion.insert(p1);    
            }
        }
        return setUnion;
    }
    void postOrder(const beemaster::pattern& root, vector<beemaster::pattern> nodes){    
        if(!root.children.empty()){
            for(auto& child : root.children){ 
                std::postOrder(child, nodes);
            }
        } else if(std::find(nodes.begin(), nodes.end(), root) == nodes.end()) {
            nodes.push_back(root);
        } 
    }
}

namespace beemaster{
    pattern::pattern(){
        this->srcIp = "";
        this->srcPrt = 0;
        this->dstPrt = 0;
        this->protocol = "";
        this->count = 0;
        this->support = 0;
        this->signature = "";
        this->remaining = 0;
        this->type = 0;
        this->isLeaf = false;
        this->parents = {};
        this->children = {};
    } 
    inline bool beemaster::pattern::operator==(const beemaster::pattern& p1) const{
        return hash<beemaster::pattern>()(p1) == hash<beemaster::pattern>()(*this);
    }

    // from: http://stackoverflow.com/questions/24327637/what-is-the-most-efficient-c-method-to-split-a-string-based-on-a-particular-de
    vector<string>
    split( string const& original, char separator ){
        vector<string> results;
        string::const_iterator start = original.begin();
        string::const_iterator end = original.end();
        string::const_iterator next = find(start, end, separator);
        while ( next != end ) {
            results.push_back( string( start, next ) );
            start = next + 1;
            next = find( start, end, separator );
        }
        results.push_back( string( start, next ) );
        return results;
    }

    beemaster::pattern root;
    vector<beemaster::pattern> type2;
    vector<beemaster::pattern> type3;
    vector<beemaster::pattern> type4;
    vector<beemaster::pattern> type5;
    vector<beemaster::pattern> type6;
    vector<beemaster::pattern> type7;
    vector<beemaster::pattern> type8;
    // struct dbInfos = 0;
    // alle patterns nach paper hardcoded
    std::vector<string> patternTypes = {"srcIp", "srcIp:srcPrt", "srcIp:dstPrt", "srcIp:protocol", "srcIp:srcPrt:dstPrt", "srcIp:srcPrt:protocol", "srcIp:dstPrt:protocol", "srcIp:srcPrt:dstPrt:protocol"};
    // generate pattern for a certain patternType and alert. Map from alert all members according to patterntype to pattern
    beemaster::pattern* beemaster::LatticeCorrelation::generatePattern(acu::IncomingAlert a, string patternSignature, int alertsSize){
        beemaster::pattern* p = new beemaster::pattern;
        //beemaster::pattern p2;
        //p = &p2;
        p->count = p->count+1;
        p->support = p->count / float(alertsSize);
        auto elements = split(patternSignature, ':');
        for (auto element : elements){
            //TODO refactor looks shitty
            if(element == "srcIp"){
                p->srcIp = a.source_ip();
            } else if (element == "srcPrt"){
                p->srcPrt = a.source_port();
            } else if (element == "dstPrt"){
                p->dstPrt = a.destination_port();
            } else if (element == "protocol"){
                p->protocol = a.protocol();
            }
        }
        ptrdiff_t pos = find(patternTypes.begin(), patternTypes.end(), patternSignature) - patternTypes.begin();
        p->type = pos+1;
        if(p->type == 8){
            p->isLeaf = true;
        }
        p->signature = patternSignature;
        return p;
    }
    void beemaster::LatticeCorrelation::generateNodesRelation(unordered_set<beemaster::pattern*>* p1){
    // TODO: refactor, optimize, FUCK MY ASS
        for(auto pattern1 : *p1){
            for(auto pattern2 : *p1){
                if(pattern1->type == 1){
                    if(pattern2->type == 2 || pattern2->type == 3 || pattern2->type == 4){
                        pattern2->parents.push_back(*pattern1);
                        pattern1->children.push_back(*pattern2);
                    }
                }
                if(pattern1->type == 2){
                    if(pattern2->type == 5 || pattern2->type == 6){
                        printf("added!");
                        pattern2->parents.push_back(*pattern1);
                        pattern1->children.push_back(*pattern2);
                    }
                }
                if(pattern1->type == 3){
                    if(pattern2->type == 5 || pattern2->type == 7){
                        pattern2->parents.push_back(*pattern1);
                        pattern1->children.push_back(*pattern2);
                    }
                }
                if(pattern1->type == 4){
                    if(pattern2->type == 6 || pattern2->type == 7){
                        pattern2->parents.push_back(*pattern1);
                        pattern1->children.push_back(*pattern2);
                    }
                }
                if(pattern1->type == 5){
                    if(pattern2->type == 8){
                        pattern2->parents.push_back(*pattern1);
                        pattern1->children.push_back(*pattern2);
                    }
                }
                if(pattern1->type == 6){
                    if(pattern2->type == 8){
                        pattern2->parents.push_back(*pattern1);
                        pattern1->children.push_back(*pattern2);
                    }
                }
                if(pattern1->type == 7){
                    if(pattern2->type == 8){
                        pattern2->parents.push_back(*pattern1);
                        pattern1->children.push_back(*pattern2);
                    }    
                }
            }
        }
    }

    acu::OutgoingAlert* beemaster::LatticeCorrelation::Invoke(){
        //this->correlate();
        auto o = acu::OutgoingAlert("test", std::chrono::system_clock::now()) ;
        return &o;
    }
    unordered_set<beemaster::pattern> beemaster::LatticeCorrelation::correlate(vector<acu::IncomingAlert> alerts, int threshold){
        // init set of patterns that will be returned
        unordered_set<pattern> patterns;
        // init lattices indexed by ip. Here a request to storage needs to be done
        // e.g get all entries from db holen, ip is key, pattern is value
        // TODO: DB call here: can I regex on all keys in rocksdb?
        //
        // All Lattice, use srcIp as Key
        unordered_map <std::string, unordered_set<pattern*>> lattice = {};

        unordered_set<pattern*> lattice_ip;
        unordered_set<pattern> patterns_ip;
        for(std::size_t i = 0; i < alerts.size(); i++){
            acu::IncomingAlert currAlert = alerts[i];
            string ip = currAlert.source_ip();
            int it = lattice.count(ip);
            if(it == 0){
                // create lattice with ip
                lattice_ip = {};
            } else {
                // get element from lattice
                lattice_ip = lattice.find(ip)->second;
            }
            // generate pattern, for all pattern types
            for(string patternType : patternTypes){
                //TODO: If pattern exists just update support val
                lattice_ip.insert(generatePattern(currAlert, patternType, alerts.size())) ;
            }
            lattice[ip] = lattice_ip;
        }
        // filtering process
        // mining significant pattern instances
        for(auto& lattice_ip : lattice){
            auto lattice_ip2 = lattice_ip.second;
            for(auto it : lattice_ip2){
                if(it->support < threshold){
                    // pattern is insignificant -> delete
                    lattice_ip2.erase(it);
                }
            }
        }
        // filtering redundant pattern instances
        // init non-redundant significant pattern instance set
        for(auto& lattice_ip : lattice){
            // compress revised Lattice lattice_ip using threshold
            std::vector<unordered_set<beemaster::pattern>> sets;
            sets.push_back(patterns);
            sets.push_back(this->latticeCompression(lattice_ip.second, threshold));
            patterns = merge_set(sets);
        }
        return patterns;
    }
    
    unordered_set<beemaster::pattern> beemaster::LatticeCorrelation::latticeCompression(unordered_set<beemaster::pattern*> lattice_ip, int threshold){
        unordered_set<beemaster::pattern> patterns;
        std::vector<beemaster::pattern> nodes;
        this->generateNodesRelation(&lattice_ip);
        std::postOrder(root, nodes);
        for(beemaster::pattern pattern1 : nodes){
                    
            if(pattern1.isLeaf){
                pattern1.remaining = pattern1.support;
            } else {
                pattern1.remaining = 0;
                for(beemaster::pattern pChild : pattern1.children){
                    pattern1.remaining += pChild.remaining;
                }
            }
            if(pattern1.remaining >= threshold) {
                patterns.insert(pattern1);
                pattern1.remaining = 0;
            }
        }
        return patterns;            
    }            
}
