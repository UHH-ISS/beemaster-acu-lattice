//
// Created by flx on 25.01.17.
//

#include <acu/aggregation.h>
#include <iostream>
#include "batch_aggregation.h"

namespace beemaster {

    bool BatchAggregation::Invoke(const acu::IncomingAlert *alert) {
        std::cout << "Aggregation invoked" << std::endl;
        ++invokes;
        return invokes >= thresholds->at(0).count;
    }
}