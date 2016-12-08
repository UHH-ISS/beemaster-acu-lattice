//
// Created by florian on 12/1/16.
//

#ifndef ACU_FW_STORAGE_H
#define ACU_FW_STORAGE_H

#include "acu/incoming_alert.h"

namespace acu {

    class Storage {
    public:
        virtual void Persist(IncomingAlert alert) = 0;
    };
}


#endif //ACU_FW_STORAGE_H
