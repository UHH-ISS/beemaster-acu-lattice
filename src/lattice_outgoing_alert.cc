#include "lattice_outgoing_alert.h"

namespace beemaster {

    const broker::message beemaster::LatticeOutgoingAlert::ToMessage() {
        //TODO: incidents is a vector of incidents, how to handle
        auto ts = std::chrono::duration_cast<std::chrono::duration<double>>(timestamp.time_since_epoch()).count();
        auto msg = broker::message{"Beemaster::lattice_result", broker::time_point(ts), incident};
        //TODO: use if viable again
        //auto msg = acu::OutgoingAlert::ToMessage();
        return msg;
    }
}
