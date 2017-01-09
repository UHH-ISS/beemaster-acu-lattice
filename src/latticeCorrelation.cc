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
    template<> struct hash<beemaster::pattern> {
        std::size_t operator()(const beemaster::pattern& k) const{
            // Compute individual hash values for first,
            // second and third and combine them using XOR
            // and bit shifting:
            return (hash<string>()(k.srcIp)
                    ^ hash<int>()(k.srcPrt)
                    ^ hash<int>()(k.dstPrt)
                    ^ hash<string>()(k.protocol)) ;
        }
    };    
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
    beemaster::pattern beemaster::LatticeCorrelation::generatePattern(acu::IncomingAlert a, string patternSignature, int alertsSize){
        beemaster::pattern p;
        ++p.count;
        p.support = p.count / float(alertsSize);
        auto elements = split(patternSignature, ':');
        for (auto element : elements){
            //TODO refactor looks shitty
            if(element == "srcIp"){
                p.srcIp = a.source_ip();
            } else if (element == "srcPrt"){
                p.srcPrt = a.source_port();
            } else if (element == "dstPrt"){
                p.dstPrt = a.destination_port();
            } else if (element == "protocol"){
                p.protocol = a.protocol();
            }
        }
        ptrdiff_t pos = find(patternTypes.begin(), patternTypes.end(), patternSignature) - patternTypes.begin();
        p.type = pos+1;
        p.signature = patternSignature;
        return p;
    }
    void beemaster::LatticeCorrelation::generateNodesRelation(unordered_set<beemaster::pattern> p1){
    // TODO: refactor, optimize, FUCK MY ASS
        for(auto& pattern1 : p1){
            switch(pattern1.type){
                case 1 :
                    root = pattern1;
                case 2:
                    type2.push_back(pattern1);
                case 3:
                    type3.push_back(pattern1);
                case 4:
                    type4.push_back(pattern1);
                case 5:
                    type5.push_back(pattern1);
                case 6:
                    type6.push_back(pattern1);
                case 7:
                    type7.push_back(pattern1);
                case 8:
                    type8.push_back(pattern1);
            }         
        }
        // Build Relations
        // root
        std::vector<pattern> rootChilds;
        rootChilds.insert(rootChilds.end(), type2.begin(), type2.end());
        rootChilds.insert(rootChilds.end(), type3.begin(), type3.end());
        rootChilds.insert(rootChilds.end(), type4.begin(), type4.end());
        root.children = rootChilds;
        // all edges from 2 to 5
        for(auto& pattern2 : type2){
            // add just childs to pattern that haven any parent attr in common
            for(auto& pattern5 : type5){
                if(pattern2.srcPrt == pattern5.srcPrt){
                    pattern2.children.push_back(pattern5);
                    pattern5.parents.push_back(pattern2);
                }    
            }
        }
        // all edges from 2 to 6
        for(auto& pattern2 : type2){
            // add just childs to pattern that haven any parent attr in common
            for(auto& pattern6 : type6){
                if(pattern2.srcPrt == pattern6.srcPrt){
                    pattern2.children.push_back(pattern6);
                    pattern6.parents.push_back(pattern2);
                }    
            }
        }
        // all edges from 3 to 5
        for(auto& pattern3 : type3){
            // add just childs to pattern that haven any parent attr in common
            for(auto& pattern5 : type5){
                if(pattern3.srcPrt == pattern5.srcPrt){
                    pattern3.children.push_back(pattern5);
                    pattern5.parents.push_back(pattern3);
                }    
            }
        }
        // 3 to 7
        for(auto& pattern3 : type3){
            // add just childs to pattern that haven any parent attr in common
            for(auto& pattern7 : type7){
                if(pattern3.srcPrt == pattern7.srcPrt){
                    pattern3.children.push_back(pattern7);
                    pattern7.parents.push_back(pattern3);
                }    
            }
        }
        // 4 to 6
        for(auto& pattern4 : type4){
            // add just childs to pattern that haven any parent attr in common
            for(auto& pattern6 : type6){
                if(pattern4.srcPrt == pattern6.srcPrt){
                    pattern4.children.push_back(pattern6);
                    pattern6.parents.push_back(pattern4);
                }    
            }
        }
        // 4 to 7
        for(auto& pattern4 : type4){
            // add just childs to pattern that haven any parent attr in common
            for(auto& pattern7 : type7){
                if(pattern4.srcPrt == pattern7.srcPrt){
                    pattern4.children.push_back(pattern7);
                    pattern7.parents.push_back(pattern4);
                }    
            }
        }
        // 5 to 8
        for(auto& pattern2 : type5){
            // add just childs to pattern that haven any parent attr in common
            for(auto& pattern5 : type8){
                if(pattern2.srcPrt == pattern5.srcPrt){
                    pattern2.children.push_back(pattern5);
                    pattern5.parents.push_back(pattern2);
                }    
            }
        }
        // 6 to 8
        for(auto& pattern2 : type6){
            // add just childs to pattern that haven any parent attr in common
            for(auto& pattern5 : type8){
                if(pattern2.srcPrt == pattern5.srcPrt){
                    pattern2.children.push_back(pattern5);
                    pattern5.parents.push_back(pattern2);
                }    
            }
        }
        // 7 to 8
        for(auto& pattern2 : type7){
            // add just childs to pattern that haven any parent attr in common
            for(auto& pattern5 : type8){
                if(pattern2.srcPrt == pattern5.srcPrt){
                    pattern2.children.push_back(pattern5);
                    pattern5.parents.push_back(pattern2);
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
        unordered_map <std::string, unordered_set<pattern>> lattice = {};

        unordered_set<pattern> lattice_ip;
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
            for(auto it = lattice_ip2.begin(); it != lattice_ip2.end();){
                if(it->support < threshold){
                    // pattern is insignificant -> delete
                    it = lattice_ip2.erase(it);
                } else {
                    it++;
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
    
    unordered_set<beemaster::pattern> beemaster::LatticeCorrelation::latticeCompression(unordered_set<beemaster::pattern> lattice_ip, int threshold){
        unordered_set<beemaster::pattern> patterns;
        std::vector<beemaster::pattern> nodes;
        this->generateNodesRelation(lattice_ip);
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
