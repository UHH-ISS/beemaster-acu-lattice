//
// Created by florian on 12/1/16.
//

// Lattice is a map, key is ip, value is set of patterns
// Rocksdb benutzt ips als key und patterns als value. ggf. ip:j, wobei j dast j-te pattern ist. Siehe paper.
// TODO: Welchen Datentyp soll pattern haben

#include "acu/correlation.h"
#include "acu/lattice.h"
#include <acu/Storage>
#include <unordered_set>
#include <unordered_map>
#include <string>

using namespace acu:
using namespace std:

class LatticeCorrelation: public Correlation{
        int thresholds[] = [10];
        string topic = "/acu/scans";
        struct dbInfos = 0;
        // TODO: Implement generate pattern method
        generatePattern(string ip);
    public:
        unordered_set Invoke(struct alerts){
            // init set of patterns that will be returned
            unordered_set<T> patterns;
            // init lattices indexed by ip. Here a request to storage needs to be done
            // e.g get all entries from db holen, ip is key, pattern is value
            unordered_map<string, unordered_set> lattice_ip = Storage.get()
                for(IncomingAlert alert: alerts){
                    ip = IncomingAlert.sourceIP;
                    it = lattice_ip.find(ip);
                    if(it != lattice_ip.end()){
                        // create lattice with ip
                        // TODO: do generate pattern here, instead with the extra loop
                        unordered_set<T> patterns_ip;
                    }
                    for(int i = 0; i < 8; i++){
                        //TODO: Generate Pattern Method, probably for loop could be discarded
                        patterns2_ip = generatePattern(alert) ;
                        // TODO: Support calc missing for lattice_ip["<patternSignature>"].support = lattice_ip["<patternSignature>"].count / alerts.size()
                    }
                }
            // filtering process
            // mining significant pattern instances
            for(auto lattices : lattice_ip){
                for(<T> pattern :lattices){
                    if(pattern.support < thresholds[0]){
                        // pattern is insignificant -> delete
                        lattices.erase(pattern.signature);
                    }
                }
            }
            // filtering redundant pattern instances
            // init non-redundant significant pattern instance set
            for(auto lattice : lattice_ip){
                // compress revised Lattice lattice_ip using threshold
                // TODO: merge patterns
                patterns += latticeCompression(lattice, thresholds[0]);
            }
        return patterns;
        }
        unordered_set latticeCompression(unordered_set<T> lattice, int threshold){
            unordered_set<T> patterns;
            for(unordered_set pattern : Node){
                if(pattern is leaf){
                    pattern.remaining = pattern.support;
                } else {
                    pattern.remaining = 0;
                    for(pattern.child : pattern.children){
                        pattern.remaining += pattern.child.reamining;
                    }
                }
                if(pattern.remaining >= threshold) {
                    patterns.insert(pattern);
                    pattern.remaining = 0;
                }
            }
            return patterns;            
        }
        
}
