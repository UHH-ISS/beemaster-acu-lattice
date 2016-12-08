//
// Created by florian on 12/1/16.
//

#ifndef ACU_FW_TRESHOLD_H
#define ACU_FW_TRESHOLD_H

#include <string>

namespace acu {

    struct Threshold {
    public:
        int count;
        std::string field_name;
        std::string value;
    };
}


#endif //ACU_FW_TRESHOLD_H
