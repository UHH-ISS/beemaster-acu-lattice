/* alert_mapper.cc
 * ACU Framework
 *
 * <include/acu/alert_mapper.h>
 */

#include "lattice_alert_mapper.h"

namespace beemaster {

    acu::IncomingAlert *LatticeAlertMapper::GetAlert(const std::string *topic, const broker::message &msg) const {
        return new LatticeIncomingAlert(topic, msg);
    }
}
