#include "lattice_incoming_alert.h"

namespace beemaster {

    LatticeIncomingAlert::LatticeIncomingAlert(const std::string *topic, const broker::message &msg) : acu::IncomingAlert(topic, msg) {
        /* The message format uses a mandatory record as the first field of the message
         * which contains 5 fields which are present in every alert. */

        // Require at least one field
        assert(!topic->empty());
        assert(msg.size() >= 1);

        // Require this field to be a broker::record with exactly 5 items
        assert(broker::is<broker::record>(message[0]));
        auto rec = broker::get<broker::record>(message[0]);
        assert(rec->size() == 6);
        this->attributes = {};
        this->attributes.insert({"test", "test"});
        this->attributes.insert({"srcIp",*broker::get<std::string>(rec->get(1).get())});
        this->attributes.insert({"srcPrt",std::to_string(*broker::get<uint64_t>(rec->get(2).get()))});
        this->attributes.insert({"dstIp",*broker::get<std::string>(rec->get(3).get())});
        this->attributes.insert({"dstPrt",std::to_string(*broker::get<uint64_t>(rec->get(4).get()))});
        this->attributes.insert({"protocol",*broker::get<std::string>(rec->get(5).get())});
        // TODO: We could also "typecheck" the message fields here to fail early?
    }

   

    std::string LatticeIncomingAlert::getAttribute(std::string key)  {
        return this->attributes.at(key);
    }

    bool LatticeIncomingAlert::operator==(const LatticeIncomingAlert &rhs) const {
        return message == rhs.message;
    }

    bool LatticeIncomingAlert::operator!=(const LatticeIncomingAlert &rhs) const {
        return !(*this == rhs);
    }
}