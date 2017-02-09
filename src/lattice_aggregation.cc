#include "lattice_aggregation.h"

namespace beemaster {
    LatticeAggregation::LatticeAggregation(acu::Storage *storage, std::vector<acu::Threshold> *thresholds)
            : Aggregation(storage, thresholds) {
        count = 0;
    }

    bool LatticeAggregation::Invoke(const acu::IncomingAlert *alert) {
        ++count;
        for (auto threshold : *thresholds) {
            if (threshold.count == count) {
                count = 0;
                return true;
            }
        }
        return false;
    }
}
