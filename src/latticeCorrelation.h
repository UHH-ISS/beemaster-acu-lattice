#ifndef ACU_IMPL_LATTICECORRELATION_H
#define ACU_IMPL_LATTICECORRELATION_H
#include <acu/correlation.h>
#include "rocks_storage.h"
#include "vector_storage.h"
#include <unordered_set>
#include <algorithm>
#include <acu/outgoing_alert.h>
#include <acu/incoming_alert.h>
namespace beemaster {
        struct pattern {
            int count;
            int remaining;
            std::string srcIp;
            int srcPrt;
            int dstPrt;
            std::string protocol;
            float support;
            int type;
            std::string signature;
            bool isLeaf;
            std::vector<pattern*> parents;
            std::vector<pattern*> children;
            std::string key;
            inline bool operator==(const pattern& p1) const;
            pattern(); 
    };
    class LatticeCorrelation : public acu::Correlation {
        public:
        beemaster::VectorStorage* db2;
        std::unordered_map<std::string, std::string> attackMap;
        pattern* generatePattern(acu::IncomingAlert alert, std::string patternSignature, int alertsSize);
    void generateNodesRelation(std::unordered_set<pattern*>* pattern);


            LatticeCorrelation(beemaster::VectorStorage* vStorage, beemaster::RocksStorage<int>* storage, std::vector<acu::Threshold>* thresholds) : acu::Correlation(storage, thresholds) {
        this->db2 = vStorage;
        this->attackMap = {{"1","scan"},{"2","Flash crowds response"}, {"3","Trinoo DDoS"}, {"4","worm"}, {"5","reflector DDoS"}, {"6","SYN flood response"}, {"7","W32/Blast worm"}, {"8","SQL-Slammer worm"}};
            };
            std::unordered_set<pattern*>* correlate(std::vector<const acu::IncomingAlert*> alerts, int threshold);
            std::unordered_set<pattern*>* latticeCompression(std::unordered_set<pattern*>* lattice, int threshold);
            acu::OutgoingAlert* Invoke();
    };

}
namespace std{
    template<> struct hash<beemaster::pattern> {
        std::size_t operator()(const beemaster::pattern& k) const{
            // Compute individual hash values for first,
            // second and third and combine them using XOR
            // and bit shifting:
            printf("hash used!\n");
            return (hash<string>()(k.srcIp)
                    ^ hash<int>()(k.srcPrt)
                    ^ hash<int>()(k.dstPrt)
                    ^ hash<string>()(k.protocol)
                    ^ hash<int>()(k.type)) ;
        }
    };
    // Merge any unordered_set<pattern> instances
    unordered_set<beemaster::pattern> merge_set(const std::vector<unordered_set<beemaster::pattern>>& p);
    inline void postOrder(beemaster::pattern* root, vector<beemaster::pattern*>* nodes){
        if(!root->children.empty()){
            for(auto& child : root->children){ 
                std::postOrder(child, nodes);
            }
        } if(std::find(nodes->begin(), nodes->end(), root) == nodes->end()) {
            nodes->push_back(root);
        } 
    }

}   

#endif // ACU_IMPL_LATTICECORRELATION_H
