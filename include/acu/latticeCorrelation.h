//
// Created by florian on 12/1/16.
//

#ifndef ACU_FW_LATTICECORRELATION_H
#define ACU_FW_LATTICECORRELATION_H
#include "correlation.h"
namespace acu {

    class latticeCorrelation : public Correlation {
    public:
        latticeCorrelation();

        virtual void Invoke() = 0;
    };
}


#endif //ACU_FW_LATTICECORRELATION_H
