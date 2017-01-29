/* vector_storage.h
 * ACU Implementation
 *
 * Simple topic-to-messages database.
 *
 * @author: 1jost, 5jendryc
 */

#ifndef ACU_IMPL_VECTORSTORAGE_H
#define ACU_IMPL_VECTORSTORAGE_H

#include "lattice_incoming_alert.h"
#include <acu/incoming_alert.h>
#include <acu/storage.h>
#include <vector>
#include <unordered_map>
#include <cstdint>

namespace beemaster {

    class VectorStorage : public acu::Storage {
    public:
        /// Initialise DB
        VectorStorage(std::string db_name);
        virtual ~VectorStorage();

        /// Persist an alert
        void Persist(const acu::IncomingAlert *alert);
        
        // Persist LatticeAlert
        void LatticePersist(const beemaster::LatticeIncomingAlert *alert);
        /// Return all messages for *topic* and remove them from the DB
        std::vector<const beemaster::LatticeIncomingAlert*>* Pop(const std::string topic);

    private:
        std::unordered_map<std::string, std::vector<const beemaster::LatticeIncomingAlert*>*>* map;
    };
}

#endif //ACU_IMPL_VECTORSTORAGE_H
