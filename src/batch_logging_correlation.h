//
// Created by flx on 25.01.17.
//

#ifndef ACU_IMPL_BATCH_LOGGING_CORRELATION_H
#define ACU_IMPL_BATCH_LOGGING_CORRELATION_H

#include <acu/correlation.h>

namespace beemaster {

    class BatchLoggingCorrelation : public acu::Correlation {
    public:
        BatchLoggingCorrelation(acu::Storage *storage, std::vector<acu::Threshold> *thresholds)
                : acu::Correlation(storage, thresholds) {};

        acu::OutgoingAlert *Invoke();
    };
}
#endif //ACU_IMPL_BATCH_LOGGING_CORRELATION_H
