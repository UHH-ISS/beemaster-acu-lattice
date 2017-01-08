/* rocks_storage.h
 * ACU Implementation
 *
 * Provide a interface to RocksDB as an implementation of
 * abstract Storage class.
 *
 * @author: 1jost, 1wilkens
 */

#ifndef ACU_IMPL_ROCKSSTORAGE_H
#define ACU_IMPL_ROCKSSTORAGE_H

#include <acu/storage.h>
#include <rocksdb/db.h>
#include <cstdint>

namespace beemaster {

    class RocksStorage : public acu::Storage {
    public:
        rocksdb::DB* Database;
        rocksdb::Options Options;

        /// Initialise and open DB
        ///
        /// @param db_name  The path for the database
        RocksStorage(std::string db_name);
        virtual ~RocksStorage();

        /// Persist an alert
        ///
        /// -- NOT IMPLEMENTED --
        void Persist(const acu::IncomingAlert *alert);

        /// Increment by *value*.
        ///
        /// @typeparam count_t  The count type (number type)
        ///
        /// @param key      The key which value will be incremented
        /// @param value    The value to increment by
        ///
        /// @return         Whether the increment was successful or not
        template<typename count_t>
        bool Increment(const std::string key, const count_t value);

        /// Get the value of *key*.
        ///
        /// @typeparam count_t  The count type (number type)
        ///
        /// @param key      The key which value will be returned
        ///
        /// @return         The stored value
        template<typename count_t>
        count_t Get(const std::string key);
    };
}

#endif //ACU_IMPL_ROCKSSTORAGE_H
