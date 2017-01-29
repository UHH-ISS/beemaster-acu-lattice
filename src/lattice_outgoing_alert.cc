#include "lattice_outgoing_alert.h"

namespace beemaster {

    const broker::message beemaster::LatticeOutgoingAlert::ToMessage() {
        //TODO: incidents is a vector of incidents, how to handle
        return broker::message{timestamp.time_since_epoch().count(), "test"};
    }
}
