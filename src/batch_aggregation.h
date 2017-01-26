//
// Created by flx on 25.01.17.
//

#ifndef ACU_IMPL_LOGGING_AGGREGATION_H
#define ACU_IMPL_LOGGING_AGGREGATION_H


#include <acu/aggregation.h>

namespace beemaster {

    class BatchAggregation : public acu::Aggregation {
    public:
        BatchAggregation(acu::Storage *storage, std::vector<acu::Threshold> *thresholds)
            : acu::Aggregation(storage, thresholds), invokes(0) {};
        bool Invoke(const acu::IncomingAlert *alert);
    private:
        uint16_t invokes;
    };

}
#endif //ACU_IMPL_LOGGING_AGGREGATION_H
