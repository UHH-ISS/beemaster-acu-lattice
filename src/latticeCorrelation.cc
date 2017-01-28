//
// Created by florian on 12/1/16.
//
#include "latticeCorrelation.h"
#include "lattice_outgoing_alert.h"
#include <acu/incoming_alert.h>
#include "lattice_threshold.h"
#include <unordered_map>
#include <string>
#include <iostream>
#include <vector>
#include <iterator>
#include <chrono>
using namespace std;
namespace beemaster{
    pattern::pattern(){
        this->attributes = {};
        this->count = 0;
        this->support = 0;
        this->remaining = 0;
        this->type = 0;
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
    // all pattern types according to paper
    std::vector<string> patternTypes = {"srcIp", "srcIp:srcPrt", "srcIp:dstPrt", "srcIp:protocol", "srcIp:srcPrt:dstPrt", "srcIp:srcPrt:protocol", "srcIp:dstPrt:protocol", "srcIp:srcPrt:dstPrt:protocol"};
    // generate pattern for a certain patternType and alert. Map from alert all members according to patterntype to pattern
    beemaster::pattern* beemaster::LatticeCorrelation::generatePattern(acu::IncomingAlert a, string patternSignature, int alertsSize){
        beemaster::pattern* p = new beemaster::pattern;
        ptrdiff_t pos = find(patternTypes.begin(), patternTypes.end(), patternSignature) - patternTypes.begin();
        p->type = pos+1;
        p->key = std::to_string(p->type);
        auto elements = split(patternSignature, ':');
        for (auto element : elements){
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
        p->count = 1;
        p->support = p->count / float(alertsSize);
        return p;
    }
    void beemaster::LatticeCorrelation::generateNodesRelation(unordered_map<std::string, beemaster::pattern*>* p1){
        for(auto el1 : *p1){
            for(auto el2 : *p1){
                bool eq = true;
                auto pattern1 = el1.second;
                auto pattern2 = el2.second;
                for(auto keyIt = pattern1->attributes.begin(); keyIt != pattern1->attributes.end(); ++keyIt){   
                    if(pattern2->attributes.count(keyIt->first) == 0 || pattern2->attributes.size()-pattern1->attributes.size() != 1){
                        eq = false;
                        break;
                    } 
                    else if(pattern2->attributes[keyIt->first] != keyIt->second){
                        eq = false;
                        break;
                    }
                    else if(pattern1->key == pattern2->key){
                        eq = false;
                        break;
                    }
                }
                if(eq){ 
                    pattern1->children.push_back(pattern2);
                    pattern2->parents.push_back(pattern1);
                }                
            }
        }
    }

    beemaster::LatticeOutgoingAlert* beemaster::LatticeCorrelation::Invoke(){
        beemaster::LatticeOutgoingAlert* o = nullptr;
        auto alerts = this->vStorage->Pop(this->topic);
        std::vector<std::string> incs = {};
        for(auto threshold : *this->latticeThresholds){ 
            auto res = this->correlate(*alerts,threshold.countRatio);
            for(auto pattern : *res){
                incs.push_back(this->attackMap.at(pattern.second->type));
            }
            o = new beemaster::LatticeOutgoingAlert(incs, std::chrono::system_clock::now());
        }
        return o;
    }
    unordered_map<std::string, beemaster::pattern*>* beemaster::LatticeCorrelation::correlate(vector<const acu::IncomingAlert*> alerts, float threshold){
        // init set of patterns that will be returned
        auto patterns = new unordered_map<std::string, pattern*>;
        // All Lattice, use srcIp as Key
        unordered_map <std::string, unordered_map<std::string, pattern*>*> lattice = {};
        unordered_map<std::string, pattern*>* lattice_ip;
        for(auto alert : alerts){
            string ip = alert->source_ip();
            int it = lattice.count(ip);
            if(it == 0){
                // create lattice with ip
                lattice_ip = new unordered_map<std::string, pattern*>;
            } else {
                // get element from lattice
                lattice_ip = lattice.at(ip);
            }
            // generate pattern, for all pattern types
            for(string patternType : patternTypes){
                auto newPattern = generatePattern(*alert, patternType, alerts.size());
                // check if the pattern already exists and update it if it does.
                auto update = lattice_ip->insert({newPattern->key, newPattern});
                if (!update.second) {
                    update.first->second->count = update.first->second->count+1;
                    update.first->second->support = update.first->second->count / (float) alerts.size();
                }
                
            }
            lattice[ip] = lattice_ip;
        }
        // filtering process
        // mining significant pattern instances
        for(auto& lattice_ip : lattice){
            auto lattice_ip2 = lattice_ip.second;
            for(auto it = lattice_ip2->begin(); it != lattice_ip2->end();){
                auto val = it->second;
                auto supp = val->support;
                if(supp < threshold){
                    // pattern is insignificant -> delete
                    it = lattice_ip2->erase(it);
                } else {
                    ++it;
                }
            }
        }
        // filtering redundant pattern instances
        // init non-redundant significant pattern instance set
        for(auto& lattice_ip : lattice){
            // compress revised Lattice lattice_ip using threshold
            auto comp = this->latticeCompression(lattice_ip.second, threshold);
            patterns->insert(comp->begin(), comp->end());
        }
        return patterns;
    }
    
    unordered_map<std::string, beemaster::pattern*>* beemaster::LatticeCorrelation::latticeCompression(unordered_map<std::string, beemaster::pattern*>* lattice_ip, float threshold){
        unordered_map<std::string, beemaster::pattern*>* patterns = new unordered_map<std::string, beemaster::pattern*>;
        std::vector<beemaster::pattern*> nodes;
        this->generateNodesRelation(lattice_ip);
        beemaster::pattern* root = new beemaster::pattern;
        for(auto pattern : *lattice_ip){
            if(pattern.second->type == 1){
                root = pattern.second;
                break;
            }
        }
        std::postOrder(root, &nodes);
        for(auto pattern1 : nodes){
            if(pattern1->children.size() == 0){
                pattern1->remaining = pattern1->support;
            } else {
                pattern1->remaining = 0;
                for(auto pChild : pattern1->children){
                    pattern1->remaining += pChild->remaining;
                }
            }
            if(pattern1->remaining >= threshold) {
                patterns->insert({pattern1->key, pattern1});
                pattern1->remaining = 0;
            }
        }
        return patterns;            
    }            
}
