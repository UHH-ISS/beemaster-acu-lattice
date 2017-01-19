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

    template <class count_t>
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
        /// @param key      The key which value will be incremented
        /// @param value    The value to increment by
        ///
        /// @return         Whether the increment was successful or not
        bool Increment(const std::string key, const count_t value);

        /// Sets the given *value*.
        ///
        /// @param key      The key which will be set
        /// @param value    The value to set
        ///
        /// @return         Whether the operation was successful or not
        bool Set(const std::string key, const count_t value);

        /// Get the value of *key*.
        ///
        /// @param key      The key which value will be returned
        ///
        /// @return         The stored value
        count_t Get(const std::string key);

        /// Returns the Iterator over the rocksdb, which allows for key-value
        /// traversal:
        ///
        /// ```
        /// for (it->SeekToFirst(); it->Valid(); it->Next()) {
        ///   ... it->key(); ... it->value();
        /// }
        /// delete it;
        /// ```
        /// Be aware, that the value is a rocksdb::Slice here. Cast via
        /// `*(count_t*)it->value().data()
        rocksdb::Iterator* GetIterator();
    };

    // http://stackoverflow.com/a/8752879
    template class RocksStorage<int>;
    template class RocksStorage<uint8_t >;
    template class RocksStorage<uint16_t>;
    template class RocksStorage<uint32_t>;
    template class RocksStorage<uint64_t>;
    template class RocksStorage<float>;
    template class RocksStorage<double>;
}

#endif //ACU_IMPL_ROCKSSTORAGE_H
