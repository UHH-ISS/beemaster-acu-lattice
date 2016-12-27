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
        int count;
        int remaining;
        string srcIp;
        int srcPrt;
        int dstPrt;
        string protocol;
        int support;
        int type;
        string signature;
        bool isLeaf;
        std::vector<pattern> parents;
        std::vector<pattern> children;
        inline bool operator==(const pattern& p1) const;        
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
    template<> struct hash<pattern> {
        std::size_t operator()(const pattern& k) const{
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
    unordered_set<pattern> merge_set(const std::vector<unordered_set<pattern>>& p){
        unordered_set<pattern> setUnion = {};
        for(auto pattern_set : p){
            for(pattern p1 : pattern_set){
                setUnion.insert(p1);    
            }
        }
        return setUnion;
    }
    void postOrder(const pattern& root, vector<pattern> nodes){    
        if(!root.children.empty()){
            for(auto& child : root.children){ 
                std::postOrder(child, nodes);
            }
        } else if(std::find(nodes.begin(), nodes.end(), root) == nodes.end()) {
            nodes.push_back(root);
        } 
    }
}
inline bool pattern::operator==(const pattern& p1) const{
    return hash<pattern>()(p1) == hash<pattern>()(*this);
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
            pattern root;
            vector<pattern> type2;
            vector<pattern> type3;
            vector<pattern> type4;
            vector<pattern> type5;
            vector<pattern> type6;
            vector<pattern> type7;
            vector<pattern> type8;
            int threshold = 5;
            string topic = "/acu/scans";
            // struct dbInfos = 0;
            // alle patterns nach paper hardcoded
            string patternsTypes[8] = {"srcIp", "srcIp:srcPrt", "srcIp:dstPrt", "srcIp:protocol", "srcIp:srcPrt:dstPrt", "srcIp:srcPrt:protocol", "srcIp:dstPrt:protocol", "srcIp:srcPrt:dstPrt:protocol"};
            // generate pattern for a certain patternType and alert. Map from alert all members according to patterntype to pattern
            pattern generatePattern(struct alert, string patternSignature, int alertsSize){
                // map
                pattern p;
                ++p.count;
                p.support = p.count / alertsSize;

                auto elements = split(patternSignature, ':');
                for (auto element : elements){
                    //TODO REBUILD alert -> pattern mapping
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
                //p.type;
                p.signature = patternSignature;
                return p;
            }
            void generateNodesRelation(unordered_set<pattern> p1){
                // TODO: refactor, optimize, FUCK MY ASS
                for(auto& pattern1 : p1){
                    switch(pattern1.type){
                        case 1 :
                            this->root = pattern1;
                        case 2:
                            this->type2.push_back(pattern1);
                        case 3:
                            this->type3.push_back(pattern1);
                        case 4:
                            this->type4.push_back(pattern1);
                        case 5:
                            this->type5.push_back(pattern1);
                        case 6:
                            this->type6.push_back(pattern1);
                        case 7:
                            this->type7.push_back(pattern1);
                        case 8:
                            this->type8.push_back(pattern1);
                    }         
                }
                // TODO: Build Relations
                // root
                std::vector<pattern> rootChilds;
                rootChilds.insert(rootChilds.end(), this->type2.begin(), this->type2.end());
                rootChilds.insert(rootChilds.end(), this->type3.begin(), this->type3.end());
                rootChilds.insert(rootChilds.end(), this->type4.begin(), this->type4.end());
                this->root.children = rootChilds;

                for(auto& pattern1 : p1){
                        
                }


            }
        public:
            void Invoke(){};
            unordered_set<pattern> Invoke(vector<alert> alerts){
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
                            lattice_ip.insert(generatePattern(currAlert, patternType, alerts.size())) ;
                        }
                    }
                // filtering process
                // mining significant pattern instances
                for(auto& lattice_ip : lattice){
                    auto lattice_ip2 = lattice_ip.second;
                    for(auto& pattern1 : lattice_ip2){
                        if(pattern1.support < threshold){
                            // pattern is insignificant -> delete
                            lattice_ip2.erase(pattern1);
                        }
                    }
                }
                // filtering redundant pattern instances
                // init non-redundant significant pattern instance set
                for(auto& lattice_ip : lattice){
                    // compress revised Lattice lattice_ip using threshold
                    std::vector<unordered_set<pattern>> sets;
                    sets.push_back(patterns);
                    sets.push_back(this->latticeCompression(lattice_ip.second, threshold));
                    patterns = merge_set(sets);
                }
            return patterns;
            }
            
            unordered_set<pattern> latticeCompression(unordered_set<pattern> lattice_ip, int threshold){
                unordered_set<pattern> patterns;
                // TODO: how to build node graph??
                // TODO: lattice_ip needs some sort of nodes datatype, probably a generate function
                // std::vector<pattern> nodes = post_order_traversal_sort(lattice_ip);
                //std::vector<pattern> nodes = std::postOrder(lattice_ip.root, nodes);    
                std::vector<pattern> nodes = {};
                for(pattern pattern1 : nodes){
                    if(pattern1.isLeaf){
                        pattern1.remaining = pattern1.support;
                    } else {
                        pattern1.remaining = 0;
                        for(pattern pChild : pattern1.children){
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
    };
}
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

