#ifndef ACU_IMPLC_LATTICE_THRESHOLD_H
#define ACU_IMPLC_LATTICE_THRESHOLD_H

#include <acu/threshold.h>

namespace beemaster {
    class LatticeThreshold : public acu::Threshold {
    public:
        float countRatio;

        LatticeThreshold(float countRatio, std::string field_name, std::string value)
                : acu::Threshold(0, field_name, value), countRatio(countRatio) {};

        bool operator==(const LatticeThreshold &) const;
        bool operator!=(const LatticeThreshold &) const;
    };
}

namespace std {

    template<>
    struct hash<beemaster::LatticeThreshold> {
        inline size_t operator()(const beemaster::LatticeThreshold &t) const {
            return hash<float>()(t.countRatio) ^ hash<string>()(t.field_name) ^ hash<string>()(t.value);
        }
    };
}

#endif // ACU_IMPLC_LATTICE_THRESHOLD_H

