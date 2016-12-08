//
// Created by florian on 11/30/16.
//

#ifndef ACU_FW_ACU_H
#define ACU_FW_ACU_H

#include "receiver.h"
#include "sender.h"

namespace acu {

    class Acu {
    public:
        Acu();

    private:
        Receiver receiver;
        Sender sender;
    };
}


#endif //ACU_FW_ACU_H
