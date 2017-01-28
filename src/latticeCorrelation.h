#ifndef ACU_IMPL_LATTICECORRELATION_H
#define ACU_IMPL_LATTICECORRELATION_H
#include <acu/correlation.h>
#include "rocks_storage.h"
#include "vector_storage.h"
#include <algorithm>
#include <unordered_map>
#include "lattice_threshold.h"
#include "lattice_outgoing_alert.h"
#include <acu/incoming_alert.h>
namespace beemaster {
        struct pattern {
            std::unordered_map<std::string, std::string> attributes;
            int count;
            float remaining;
            float support;
            int type;
            std::vector<pattern*> parents;
            std::vector<pattern*> children;
            std::string key;
            inline bool operator==(const pattern& p1) const;
            pattern(); 
    };
    class LatticeCorrelation : public acu::Correlation {
        public:
        beemaster::VectorStorage* vStorage;
        beemaster::RocksStorage<int>* storage;
        std::vector<beemaster::LatticeThreshold>* latticeThresholds;
        std::string topic;
        std::unordered_map<int, std::string> attackMap;
        pattern* generatePattern(acu::IncomingAlert alert, std::string patternSignature, int alertsSize);
    void generateNodesRelation(std::unordered_map<std::string, pattern*>* pattern);


            LatticeCorrelation(beemaster::VectorStorage* vStorage, beemaster::RocksStorage<int>* storage, std::vector<beemaster::LatticeThreshold>* latticeThresholds, std::string topic) 
                : acu::Correlation(storage, thresholds), vStorage(vStorage), storage(storage), latticeThresholds(latticeThresholds), topic(topic) {
        this->attackMap = {{1,"scan"},{2,"Flash crowds response"}, {3,"Trinoo DDoS"}, {4,"worm"}, {5,"reflector DDoS"}, {6,"SYN flood response"}, {7,"W32/Blast worm"}, {8,"SQL-Slammer worm"}};
            };
            std::unordered_map<std::string, pattern*>* correlate(std::vector<const acu::IncomingAlert*> alerts, float threshold);
            std::unordered_map<std::string, pattern*>* latticeCompression(std::unordered_map<std::string, pattern*>* lattice, float threshold);
            beemaster::LatticeOutgoingAlert* Invoke();
    };

}
namespace std{
    template<> struct hash<beemaster::pattern> {
        std::size_t operator()(const beemaster::pattern& k) const{
            // Compute individual hash values for first,
            // second and third and combine them using XOR
            // and bit shifting:
            //printf("hash used!\n");
            return hash<string>()(k.key);
        }
    };
    // Merge any unordered_set<pattern> instances
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
