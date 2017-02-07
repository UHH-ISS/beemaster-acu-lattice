#include "lattice_incoming_alert.h"

namespace beemaster {

    LatticeIncomingAlert::LatticeIncomingAlert(const std::string *topic, const broker::message &message) : acu::IncomingAlert(topic, message) {
        
        auto rec = broker::get<broker::record>(message[1]);
        this->attributes = {};
        this->attributes.insert({"srcIp",*broker::get<std::string>(rec->get(1).get())});
        this->attributes.insert({"srcPrt",std::to_string(*broker::get<uint64_t>(rec->get(2).get()))});
        this->attributes.insert({"dstPrt",std::to_string(*broker::get<uint64_t>(rec->get(3).get()))});
        this->attributes.insert({"protocol",*broker::get<std::string>(rec->get(4).get())});
    }

   

    std::string LatticeIncomingAlert::getAttribute(std::string key) const {
        return this->attributes.at(key);
    }

    bool LatticeIncomingAlert::operator==(const LatticeIncomingAlert &rhs) const {
        return message == rhs.message;
    }

    bool LatticeIncomingAlert::operator!=(const LatticeIncomingAlert &rhs) const {
        return !(*this == rhs);
    }
}
