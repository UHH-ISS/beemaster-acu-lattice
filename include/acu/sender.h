//
// Created by florian on 12/1/16.
//

#ifndef ACU_FW_SENDER_H
#define ACU_FW_SENDER_H

#include "outgoing_alert.h"

#include <string>

namespace acu {

    class Sender {
    public:
        Sender(std::string destination, uint16_t port);

        void Send(OutgoingAlert alert);
    };
}


#endif //ACU_FW_SENDER_H
