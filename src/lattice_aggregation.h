/* lattice_aggregation.h
 * ACU Lattice
 *
 * @author: 5jendryc
 */

#ifndef ACU_IMPL_LATTICE_AGGREGATIONALGORITHM_H
#define ACU_IMPL_LATTICE_AGGREGATIONALGORITHM_H

#include <acu/aggregation.h>

namespace beemaster {
    class LatticeAggregation : public acu::Aggregation {
    private:
        uint64_t count;
    public:
        LatticeAggregation(acu::Storage *storage, std::vector<acu::Threshold> *thresholds);

        bool Invoke(const acu::IncomingAlert *alert) override;
    };
}

#endif //ACU_IMPL_LATTICE_AGGREGATIONALGORITHM_H
