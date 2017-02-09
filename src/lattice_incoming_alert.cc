#include "lattice_incoming_alert.h"

namespace beemaster {

    LatticeIncomingAlert::LatticeIncomingAlert(const std::string *topic, const broker::message &message)
            : acu::IncomingAlert(topic, message) {

        this->attributes = {};
        this->attributes.insert(std::make_pair("srcIp", source_ip()));
        this->attributes.insert(std::make_pair("srcPrt", std::to_string(source_port())));
        this->attributes.insert(std::make_pair("dstPrt", std::to_string(destination_port())));
        this->attributes.insert(std::make_pair("protocol", *broker::get<std::string>(message[2])));
    }

    std::string LatticeIncomingAlert::getAttribute(std::string key) const {
        return this->attributes.at(key);
    }
}
