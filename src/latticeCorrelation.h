#ifndef ACU_IMPL_LATTICECORRELATION_H
#define ACU_IMPL_LATTICECORRELATION_H
#include <acu/correlation.h>
#include "rocks_storage.h"
#include <unordered_set>
namespace beemaster {

    class LatticeCorrelation : public acu::Correlation {
        pattern generatePattern(acu::IncomingAlert alert, std::string patternSignature, int alertsSize);
    void generateNodesRelation(std::unordered_set<pattern> pattern);


        public:
            LatticeCorrelation(beemaster::RocksStorage* storage, std::vector<acu::Threshold>* thresholds) : acu::Correlation(storage, thresholds) {}
            unordered_set<pattern> correlate(std::vector<acu::IncomingAlert> alerts, int threshold);
            unordered_set<pattern> latticeCompression(unordered_set<pattern> lattice, int threshold);
    }

}

#endif // ACU_IMPL_LATTICECORRELATION_H
