/* vector_storage.h
 * ACU Implementation
 *
 * <src/vector_storage.cc>
 */

#include "vector_storage.h"

namespace beemaster {

    VectorStorage::VectorStorage(std::string db_name)
        : acu::Storage::Storage(db_name) {
            map = new std::unordered_map<std::string, std::vector<const acu::IncomingAlert*>*>;
        }

    // Gracefully closes DB
    VectorStorage::~VectorStorage() {
        // TODO maybe we should use smart_ptr's so we do not have to this ourselves?!
        for (auto it = map->begin(); it != map->end(); ++it) {
            delete it->second;
        }
        delete map;
    }

    void VectorStorage::Persist(const acu::IncomingAlert *alert) {
        assert(alert);

        if(map->count(*alert->topic) == 0) {
            // create vector, as topic is not yet stored
            map->emplace(*alert->topic, new std::vector<const acu::IncomingAlert*>);
        }

        map->at(*alert->topic)->push_back(alert);
    }

    std::vector<const acu::IncomingAlert*>* VectorStorage::Pop(const std::string topic) {
        if(map->count(topic) == 0)
            return new std::vector<const acu::IncomingAlert*>;
        auto vector = map->at(topic);
        map->erase(topic);
        return vector;
    }
}
