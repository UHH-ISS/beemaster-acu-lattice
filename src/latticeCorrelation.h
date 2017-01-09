#ifndef ACU_IMPL_LATTICECORRELATION_H
#define ACU_IMPL_LATTICECORRELATION_H
#include <acu/correlation.h>
#include "rocks_storage.h"
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
            std::vector<pattern> parents;
            std::vector<pattern> children;
            inline bool operator==(const pattern& p1) const;
            pattern(); 
    };
    class LatticeCorrelation : public acu::Correlation {
        public:
        pattern generatePattern(acu::IncomingAlert alert, std::string patternSignature, int alertsSize);
    void generateNodesRelation(std::unordered_set<pattern> pattern);


            LatticeCorrelation(beemaster::RocksStorage* storage, std::vector<acu::Threshold>* thresholds) : acu::Correlation(storage, thresholds) {};
            std::unordered_set<pattern> correlate(std::vector<acu::IncomingAlert> alerts, int threshold);
            std::unordered_set<pattern> latticeCompression(std::unordered_set<pattern> lattice, int threshold);
            acu::OutgoingAlert* Invoke();
    };

}
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
    unordered_set<beemaster::pattern> merge_set(const std::vector<unordered_set<beemaster::pattern>>& p);
    void postOrder(const beemaster::pattern& root, vector<beemaster::pattern> nodes);
}   

#endif // ACU_IMPL_LATTICECORRELATION_H
