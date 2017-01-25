//
// Created by florian on 12/1/16.
//
#include "latticeCorrelation.h"
#include <acu/correlation.h>
//#include <acu/storage.h>
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
    unordered_set<beemaster::pattern*>* merge_set(std::vector<unordered_set<beemaster::pattern*>*> p){
        auto setUnion = new unordered_set<beemaster::pattern*>;
        for(auto p_set : p){
            //printf("it1\n");
            for(auto& p1 : *p_set) {
                //printf("it2\n");
                setUnion->insert(p1);
                //printf("was inserted!\n");    
            }
        }
        //printf("return");
        return setUnion;
    }
}

namespace beemaster{
    pattern::pattern(){
        this->attributes = {};
        //this->srcIp = "";
        //this->srcPrt = 0;
        //this->dstPrt = 0;
        //this->protocol = "";
        this->count = 0;
        this->support = 0;
        this->signature = "";
        this->remaining = 0;
        this->type = 0;
        this->isLeaf = false;
        this->parents = {};
        this->children = {};
        this->key = "";
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
    // alle patterns nach paper hardcoded
    std::vector<string> patternTypes = {"srcIp", "srcIp:srcPrt", "srcIp:dstPrt", "srcIp:protocol", "srcIp:srcPrt:dstPrt", "srcIp:srcPrt:protocol", "srcIp:dstPrt:protocol", "srcIp:srcPrt:dstPrt:protocol"};
    // generate pattern for a certain patternType and alert. Map from alert all members according to patterntype to pattern
    beemaster::pattern* beemaster::LatticeCorrelation::generatePattern(acu::IncomingAlert a, string patternSignature, int alertsSize){
        beemaster::pattern* p = new beemaster::pattern;
        ptrdiff_t pos = find(patternTypes.begin(), patternTypes.end(), patternSignature) - patternTypes.begin();
        p->type = pos+1;
        p->key = std::to_string(p->type);
        auto elements = split(patternSignature, ':');
        for (auto element : elements){
            //TODO refactor looks shitty
            if(element == "srcIp"){
                p->attributes.insert({element, a.source_ip()});
                p->key += ":" +p->attributes[element];
            } else if (element == "srcPrt"){
                p->attributes.insert({element, std::to_string(a.source_port())});
                p->key += ":" + p->attributes[element];
            } else if (element == "dstPrt"){
                p->attributes.insert({element, std::to_string(a.destination_port())});
                p->key += ":" + p->attributes[element];
            } else if (element == "protocol"){
                p->attributes.insert({element, a.protocol()});
                p->key += ":" + p->attributes[element];
            }
        }
        p->count = p->count+1;
        p->support = p->count / float(alertsSize);
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
                bool eq = true;
                //printf("pattern1: %s (%d) | pattern2: %s (%d)\n", pattern1->key.c_str(),pattern1->attributes.size(),  pattern2->key.c_str(), pattern2->attributes.size());
                for(auto keyIt = pattern1->attributes.begin(); keyIt != pattern1->attributes.end(); ++keyIt){  
                    if(pattern2->attributes.count(keyIt->first) == 0 || pattern2->attributes.size()-pattern1->attributes.size() != 1){
                        eq = false;
                        break;
                    } 
                    else if(pattern2->attributes[keyIt->first] != keyIt->second){
                        //printf("p1[%s]=%s | p2[%s]=%s =>missmatch!\n", keyIt->first.c_str(), keyIt->second.c_str(), keyIt->first.c_str(), pattern2->attributes[keyIt->first].c_str());
                        eq = false;
                        break;
                    }
                    else if(pattern1->key == pattern2->key){
                        eq = false;
                        //printf("the same!\n");
                        break;
                    }
                }
                //printf("eq:%d\n" ,eq); 
                if(eq){ 
                    pattern1->children.push_back(pattern2);
                    pattern2->parents.push_back(pattern1);
                }                
            }
        }
    }

    acu::OutgoingAlert* beemaster::LatticeCorrelation::Invoke(){
        // TODO: add a vector of types
        acu::OutgoingAlert* o = nullptr ;
        auto alerts = this->db2->Pop(this->topic);
        for(auto threshold : *this->thresholds){ 
            auto res = this->correlate(*alerts,threshold.count);
            auto it = res->begin();
            auto pat = *it;
            o = new acu::OutgoingAlert(this->attackMap.at(pat->type), std::chrono::system_clock::now()); 
        }
        return o;
    }
    unordered_set<beemaster::pattern*>* beemaster::LatticeCorrelation::correlate(vector<const acu::IncomingAlert*> alerts, int threshold){
        // init set of patterns that will be returned
        auto patterns = new unordered_set<pattern*>;
        // init lattices indexed by ip. Here a request to storage needs to be done
        // e.g get all entries from db holen, ip is key, pattern is value
        // TODO: DB call here: can I regex on all keys in rocksdb?
        //
        // All Lattice, use srcIp as Key
        //this->db->Set("test", 1337);
        /*
        auto it = this->db->GetIterator();
        for(it->SeekToFirst(); it->Valid(); it->Next()){
            printf("key: %s\n", it->key().data());
        }
        delete it;
        */
        unordered_map <std::string, unordered_set<pattern*>*> lattice = {};

        unordered_set<pattern*>* lattice_ip;
        unordered_set<pattern> patterns_ip;
        for(auto alert : alerts){
            //printf("for:188\n");
            string ip = alert->source_ip();
            int it = lattice.count(ip);
            if(it == 0){
                //printf("create lattice\n");
                // create lattice with ip
                lattice_ip = new unordered_set<pattern*>;
            } else {
                // get element from lattice
                lattice_ip = lattice.find(ip)->second;
            }
            // generate pattern, for all pattern types
            for(string patternType : patternTypes){
                //printf("generate pattern\n");
                //printf("alertSize: %d\n", alerts.size());
                //TODO: If pattern exists just update support val
                auto newPattern = generatePattern(*alert, patternType, alerts.size());
                std::unordered_set<beemaster::pattern*>::const_iterator got = lattice_ip->find (newPattern);
                //std::unordered_set<beemaster::pattern*>::hasher fn = lattice_ip->hash_function();
                //printf("%d\n", fn(newPattern));
                if(got == lattice_ip->end()) {
                    lattice_ip->insert(newPattern);
                } else {
                    printf("gefunden!");
                    auto foundPattern = *got;
                    foundPattern->count = foundPattern->count+1;
                    foundPattern->support = foundPattern->count / alerts.size();
                }
            }
            //printf("add to lattice\n");
            lattice[ip] = lattice_ip;
        }
        // filtering process
        // mining significant pattern instances
        for(auto& lattice_ip : lattice){
            //printf("mining\n");
            auto lattice_ip2 = lattice_ip.second;
            for(auto it = lattice_ip2->begin(); it != lattice_ip2->end();){
                auto val = *it;
                auto supp = val->support;
                if(supp < threshold){
                    // pattern is insignificant -> delete
                    // TODO:What about parents and children?
                    //printf("erase\n");
                    it = lattice_ip2->erase(it);
                } else {
                    ++it;
                }
            }
        }
        // filtering redundant pattern instances
        // init non-redundant significant pattern instance set
        for(auto& lattice_ip : lattice){
            //printf("filter pattern\n");
            // compress revised Lattice lattice_ip using threshold
            std::vector<unordered_set<beemaster::pattern*>*>* sets = new std::vector<unordered_set<beemaster::pattern*>*>;
            sets->push_back(patterns);
            //printf("compress\n");
            sets->push_back(this->latticeCompression(lattice_ip.second, threshold));
            patterns = merge_set(*sets);
            //printf("merge\n");
        }
        //printf("return\n");
        return patterns;
    }
    
    unordered_set<beemaster::pattern*>* beemaster::LatticeCorrelation::latticeCompression(unordered_set<beemaster::pattern*>* lattice_ip, int threshold){
        unordered_set<beemaster::pattern*>* patterns = new unordered_set<beemaster::pattern*>;
        std::vector<beemaster::pattern*> nodes;
        this->generateNodesRelation(lattice_ip);
        // print test
        beemaster::pattern* root = new beemaster::pattern;
        for(auto pattern : *lattice_ip){
            //printf("%d has %d children\n", pattern->type, pattern->children.size());
            if(pattern->type == 1){
                root = pattern;
                break;
            }
        }
        std::postOrder(root, &nodes);

        for(auto pattern1 : nodes){
            if(pattern1->isLeaf){
                pattern1->remaining = pattern1->support;
            } else {
                pattern1->remaining = 0;
                for(auto pChild : pattern1->children){
                    pattern1->remaining += pChild->remaining;
                }
            }
            if(pattern1->remaining >= threshold) {
                patterns->insert(pattern1);
                pattern1->remaining = 0;
            }
        }
        return patterns;            
    }            
}
