#ifndef ACU_IMPL_LATTICE_OUTGOING_ALERT_H
#define ACU_IMPL_LATTICE_OUTGOING_ALERT_H

#include <acu/outgoing_alert.h>
#include <chrono>
#include <string>
namespace beemaster {

    class LatticeOutgoingAlert : public acu::OutgoingAlert {
        public:
            //std::chrono::system_clock::time_point timestamp;
            std::string incident;
            LatticeOutgoingAlert(std::string incident, std::chrono::time_point<std::chrono::system_clock> timestamp)
                : acu::OutgoingAlert("beemaster/acu/acu_result", timestamp), incident(incident) {};
            
            virtual const broker::message ToMessage();
    };
}

#endif //ACU_IMPL_LATTICE_OUTGOING_ALERT_H
