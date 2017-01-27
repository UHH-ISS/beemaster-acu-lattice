#include "lattice_threshold.h"

namespace beemaster {
    bool LatticeThreshold::operator==(const LatticeThreshold &rhs) const {
        return countRatio == rhs.countRatio
            && field_name = rhs.field_name
            && value = rhs.value;
    }

    bool LatticeThreshold::operator!=(const LatticeThreshold &rhs) const {
        return !(*this == rhs);
    }
}
