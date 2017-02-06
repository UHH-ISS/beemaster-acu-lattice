/* lattice_alert_mapper.h
 * Lattice
 *
 *
 * @author: 5jendryc
 */

#ifndef ACU_IMPL_LATTICE_ALERT_MAPPER_H
#define ACU_IMPL_LATTICE_ALERT_MAPPER_H

#include "lattice_incoming_alert.h"
#include <acu/alert_mapper.h>

namespace beemaster {

    class LatticeAlertMapper: public acu::AlertMapper  {
    public:
        /// Factory method to convert a raw Broker message into the
        /// corresponding IncomingAlert-Subclass.
        ///
        /// @param topic    The topic string to identify the message.
        /// @param message  The message to convert.
        /// @return         The correctly mapped message.
        virtual acu::IncomingAlert* GetAlert(const std::string*, const broker::message&) const;
    };
}
#endif //ACU_IMPL_LATTICE_ALERT_MAPPER_H
