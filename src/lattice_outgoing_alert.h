#ifndef ACU_IMPL_LATTICE_OUTGOING_ALERT_H
#define ACU_IMPL_LATTICE_OUTGOING_ALERT_H

#include <acu/outgoing_alert.h>
#include <chrono>
#include <string>
namespace beemaster {

    class LatticeOutgoingAlert : public acu::OutgoingAlert {
        public:
            //std::chrono::system_clock::time_point timestamp;
            std::vector<std::string> incidents;
            LatticeOutgoingAlert(std::vector<std::string> incidents, std::chrono::system_clock::time_point timestamp) 
                : incidents(incidents), acu::OutgoingAlert(incidentName, timestamp) {};
    };
}

#endif //ACU_IMPL_LATTICE_OUTGOING_ALERT_H
