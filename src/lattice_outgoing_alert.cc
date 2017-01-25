#include "lattice_outgoing_alert.h"

namespace beemaster {

    const broker::message OutgoingAlert::ToMessage() {
        return broker::message{timestamp.time_since_epoch().count(), incidents};
    }
}
