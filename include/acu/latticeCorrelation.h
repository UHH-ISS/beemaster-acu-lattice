//
// Created by florian on 12/1/16.
//

#ifndef ACU_FW_LATTICECORRELATION_H
#define ACU_FW_LATTICECORRELATION_H

namespace acu {

    class latticeCorrelation {
    public:
        latticeCorrelation();

        virtual void Invoke() = 0;
    };
}


#endif //ACU_FW_LATTICECORRELATION_H
