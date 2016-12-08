//
// Created by florian on 12/1/16.
//

#ifndef ACU_FW_CORRELATION_H
#define ACU_FW_CORRELATION_H

namespace acu {

    class Correlation {
    public:
        Correlation();

        virtual void Invoke() = 0;
    };
}


#endif //ACU_FW_CORRELATION_H
