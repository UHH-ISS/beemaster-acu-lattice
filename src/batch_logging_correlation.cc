//
// Created by flx on 25.01.17.
//

#include <acu/correlation.h>
#include "batch_logging_correlation.h"
#include <iostream>

namespace beemaster {

    acu::OutgoingAlert* BatchLoggingCorrelation::Invoke() {
        std::cout << "Bingo bongo Correlation!" << std::endl;
        return nullptr;
    }
}