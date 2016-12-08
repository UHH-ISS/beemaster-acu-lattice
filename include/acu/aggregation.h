//
// Created by florian on 12/1/16.
//

#ifndef ACU_FW_AGGREGATIONALGORITHM_H
#define ACU_FW_AGGREGATIONALGORITHM_H

namespace acu {

    class Aggregation{
    public:
        Aggregation();

        virtual void Invoke() = 0;
    };
}

#endif //ACU_FW_AGGREGATIONALGORITHM_H
