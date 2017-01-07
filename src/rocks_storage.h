//
// Created by florian on 1/5/17.
//

#ifndef ACU_IMPL_ROCKSSTORAGE_H
#define ACU_IMPL_ROCKSSTORAGE_H

#include <acu/storage.h>

namespace beemaster {

    class RocksStorage : public acu::Storage {
    public:
        RocksStorage(std::string db_name) : acu::Storage::Storage(db_name) {}

        void Persist(const acu::IncomingAlert *alert);
    };
}

#endif //ACU_IMPL_ROCKSSTORAGE_H
