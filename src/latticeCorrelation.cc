//
// Created by florian on 12/1/16.
//

// Lattice is a map, key is ip, value is set of patterns
// Rocksdb speichert nicht die pattern, sondern einfach nur ein count. z.B srcIP:srcPrt -> 60.240.134.94:4313
//#include "acu/correlation.h"
//#include <acu/Storage>
#include "acu/latticeCorrelation.h"
#include <unordered_set>
#include <unordered_map>
#include <string>
#include <iostream>
#include <array>
#include <vector>
#include <iterator>
#include <algorithm>
using namespace std;
struct pattern {
        string srcIp;
        int srcPrt;
        int dstPrt;
        string protocol;
        int support;
        string type;
        string signature;
    };
// just for quick and dirty testing
struct alert {
        string srcIp;
        int srcPrt;
        int dstPrt;
        string protocol;
    };
// hash func for unordered_set
namespace std{
    template<> struct hash<pattern>{
        std::size_t operator()(const pattern& k) const{
            using std::size_t;
            using std::hash;
            using std::string;
            // Compute individual hash values for first,
            // second and third and combine them using XOR
            // and bit shifting:
            // TODO: define better hashfunc!
            return (hash<string>()(k.srcIp));
        }
    };
}
namespace acu{
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

    class LatticeCorrelation: public Correlation {
            int threshold = 5;
            string topic = "/acu/scans";
            // struct dbInfos = 0;
            // alle patterns nach paper hardcoded
            string patternsTypes[8] = {"srcIp", "srcIp:srcPrt", "srcIp:dstPrt", "srcIp:protocol", "srcIp:srcPrt:dstPrt", "srcIp:srcPrt:protocol", "srcIp:dstPrt:protocol", "srcIp:srcPrt:dstPrt:protocol"};
            pattern generatePattern(struct alert, string patternType){
                // TODO: Support calc missing for lattice_ip["<patternSignature>"].support = lattice_ip["<patternSignature>"].count / alerts.size()
                pattern p;
                auto elements = split(patternType, ':');
                for (auto element : elements){
                    //TODO REBUILD!
                    /*
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
                    */
                }
                p.type = patternType;
                return p;
            }
        public:
            void Invoke(){};
            unordered_set<pattern> Invoke(vector<alert> alerts){
                // TODO: List, Array what type is alerts?
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
                        alert currAlert = alerts[i];
                        string ip = currAlert.srcIp;
                        int it = lattice.count(ip);
                        if(it == 0){
                            // create lattice with ip
                            lattice_ip = {};
                        }
                        // generate pattern, for all pattern types
                        for(string patternType : this->patternsTypes){
                            lattice_ip.insert(generatePattern(currAlert, patternType)) ;
                        }
                    }
                // filtering process
                // mining significant pattern instances
                // auto itr = mySet.begin(); itr != mySet.end(); ++itr
                for(auto& lattice_ip : lattice){
                    auto lattice_ip2 = lattice_ip.second;
                    for(auto& pattern1 : lattice_ip2){
                        if(pattern1.support < threshold){
                            // pattern is insignificant -> delete
                            lattice_ip2.erase(pattern1);
                    }
                    //}
                }
                // filtering redundant pattern instances
                // init non-redundant significant pattern instance set
                for(auto& lattice_ip : lattice){
                    // compress revised Lattice lattice_ip using threshold
                    // TODO: merge patterns
                    //patterns += latticeCompression(lattice, threshold);
                }
            return patterns;
            }
            /*
            unordered_set<pattern> latticeCompression(unordered_set<pattern> lattice, int threshold){
                unordered_set<pattern> patterns;
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
            */
            
    };
};
/*
    int main(){
        LatticeCorrelation l;
        alert a;
        a.srcIp = "60.240.134.94";
        a.srcPrt = 4313;
        a.dstPrt = 1434;
        a.protocol = "TCP";
        vector<alert> v;
        v.push_back(a);
        auto res = l.Invoke(v);
        for(auto c : res){
        cout << 
        }
        cout << res;
        return 0;
    }
*/
}
