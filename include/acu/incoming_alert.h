//
// Created by florian on 12/1/16.
//

#ifndef ACU_FW_INCOMING_ALERT_H
#define ACU_FW_INCOMING_ALERT_H

#include <chrono>
#include <string>

#include <broker/data.hh>

namespace acu {

    class IncomingAlert {
    public:
        std::string topic;
        std::chrono::time_point<std::chrono::system_clock> timestamp;

        std::string incident_type;
        std::string protocol;

        std::string source_ip;
        uint16_t source_port;

        std::string destination_ip;
        uint16_t destination_port;
    };
}


#endif //ACU_FW_INCOMING_ALERT_H
