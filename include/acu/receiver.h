//
// Created by florian on 12/1/16.
//

#ifndef ACU_FW_RECEIVER_H
#define ACU_FW_RECEIVER_H

#include <cstdint>
#include <string>

namespace acu {

    class Receiver {
    public:
        Receiver(std::string address, uint16_t port, std::string topics[]);

        void Listen();
    };
}


#endif //ACU_FW_RECEIVER_H
