#ifndef ACU_IMPL_LATTICE_INCOMING_ALERT_H
#define ACU_IMPL_LATTICE_INCOMING_ALERT_H

#include <unordered_map>
#include <acu/utils.h>
#include <acu/incoming_alert.h>
//#include <broker/message.hh>
#include <chrono>
#include <string>

namespace beemaster {
    using namespace std::chrono;

    class LatticeIncomingAlert : public acu::IncomingAlert {
    public:
        LatticeIncomingAlert(const std::string*, const broker::message&);
            //: acu::IncomingAlert(topic,msg) {};

        //map with all necessary attributes for lattice
        std::unordered_map<std::string, const std::string> attributes;

        //give attribute back
        std::string getAttribute(std::string key) const;

        virtual bool operator==(const LatticeIncomingAlert&) const;
        virtual bool operator!=(const LatticeIncomingAlert&) const;

    protected:
        const std::vector<broker::data> message;
    };
} // namespace beemaster

#endif //ACU_IMPL_LATTICE_INCOMING_ALERT_H
