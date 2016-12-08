//
// Created by florian on 12/1/16.
//

#ifndef ACU_FW_OUTGOING_ALERT_H
#define ACU_FW_OUTGOING_ALERT_H

#include <chrono>
#include <string>
#include <broker/data.hh>

namespace acu {

    class OutgoingAlert {
    public:
        std::string name;
        std::chrono::time_point<std::chrono::system_clock> timestamp;

        virtual broker::vector AsVector();
    };
}


#endif //ACU_FW_OUTGOING_ALERT_H
