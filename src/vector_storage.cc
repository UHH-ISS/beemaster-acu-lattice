/* vector_storage.h
 * ACU Implementation
 *
 * <src/vector_storage.cc>
 */

#include "vector_storage.h"


namespace beemaster {

    VectorStorage::VectorStorage(std::string db_name)
        : acu::Storage::Storage(db_name) {
            map = new std::unordered_map<std::string, std::vector<acu::IncomingAlert*>*>;
        }

    // Gracefully closes DB
    VectorStorage::~VectorStorage() {
        delete map;
    }

    void VectorStorage::Persist(const acu::IncomingAlert *alert) {
        assert(alert);

        if(map->count(*alert->topic) == 0) {
            // create vector, as topic is not yet stored
            map->emplace(*alert->topic, new std::vector<acu::IncomingAlert*>);
        }

        auto vctr = map->at(*alert->topic);
        // TODO continue
    }

    std::vector<acu::IncomingAlert*> VectorStorage::Pop(const std::string topic) {

    }
}
