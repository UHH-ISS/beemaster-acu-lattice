//
// Created by florian on 12/1/16.
//

// Lattice is a map, key is ip, value is set of patterns
// Rocksdb speichert nicht die pattern, sondern einfach nur ein count. z.B srcIP:srcPrt -> 60.240.134.94:4313
#include "acu/correlation.h"
#include "acu/lattice.h"
#include <acu/Storage>
#include <unordered_set>
#include <unordered_map>
#include <string>

using namespace acu:
using namespace std:

struct pattern {
    string type;
    string srcIp;
    int srcPrt;
    int dstPrt;
    string protocol;
    int support;
}

// from: http://stackoverflow.com/questions/24327637/what-is-the-most-efficient-c-method-to-split-a-string-based-on-a-particular-de
vector<string>
split( string const& original, char separator )
{
    vector<string> results;
    string::const_iterator start = original.begin();
    string::const_iterator end = original.end();
    string::const_iterator next = find( start, end, separator );
    while ( next != end ) {
            results.push_back( string( start, next ) );
            start = next + 1;
            next = find( start, end, separator );
        }
    results.push_back( string( start, next ) );
    return results;
}

class LatticeCorrelation: public Correlation{
        int threshold = 5;
        string topic = "/acu/scans";
        // struct dbInfos = 0;
        // alle patterns nach paper hardcoded
        string patternsTypes[] = {"srcIp", "srcIp:srcPrt", "srcIp:dstPrt", "srcIp:protocol", "srcIp:srcPrt:dstPrt", "srcIp:srcPrt:protocol", "srcIp:dstPrt:protocol", "srcIp:srcPrt:dstPrt:protocol"};
        struct generatePattern(struct alert, string patternType){
            // TODO: Support calc missing for lattice_ip["<patternSignature>"].support = lattice_ip["<patternSignature>"].count / alerts.size()
            pattern p;
            auto elements = split(patternType, ':');
            for (auto element : elements){
                switch(element) {
                    case "srcIp" :
                        p.srcIp = alert.srcIp;
                    case "srcPrt" :
                        p.srcPrt = alert.srcPrt;
                    case "dstPrt" :
                        p.srcPrt = alert.dstPrt;
                    case "protocol" :
                        p.protocol = alert.protocol;
                }
            }
            p.type = patternType;
            return p;
        }
    public:
        auto Invoke(struct alerts){
            // TODO: List, Array what type is alerts?
            // init set of patterns that will be returned
            unordered_set<struct> patterns;
            // init lattices indexed by ip. Here a request to storage needs to be done
            // e.g get all entries from db holen, ip is key, pattern is value
            // TODO: DB call here: can I regex on all keys in rocksdb?
            unordered_map<string, unordered_set> lattice_ip = Storage.get()
                for(IncomingAlert alert: alerts){
                    ip = IncomingAlert.sourceIP;
                    it = lattice_ip.find(ip);
                    if(it == lattice_ip.end()){
                        // create lattice with ip
                        unordered_set<struct> patterns_ip;
                    }
                    // generate pattern, for all pattern types
                    for(auto patternType : patternTypes){
                        patterns_ip.insert(generatePattern(alert, patternType)) ;
                    }
                }
            // filtering process
            // mining significant pattern instances
            for(auto lattices : lattice_ip){
                for(auto pattern1 : lattices){
                    if(pattern1.support < threshold){
                        // pattern is insignificant -> delete
                        lattices.erase(pattern1.signature);
                    }
                }
            }
            // filtering redundant pattern instances
            // init non-redundant significant pattern instance set
            for(auto lattice : lattice_ip){
                // compress revised Lattice lattice_ip using threshold
                // TODO: merge patterns
                patterns += latticeCompression(lattice, threshold);
            }
        return patterns;
        }
        unordered_set latticeCompression(unordered_set<struct> lattice, int threshold){
            unordered_set<struct> patterns;
            for(unordered_set pattern1 : Node){
                if(pattern1 is leaf){
                    pattern1.remaining = pattern.support;
                } else {
                    pattern1.remaining = 0;
                    for(pattern1.child : pattern1.children){
                        pattern1.remaining += pattern1.child.reamining;
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
