/* rocks_storage.h
 * ACU Implementation
 *
 * <src/rocks_storage.cc>
 */

#include "rocks_storage.h"
#include <rocksdb/merge_operator.h>


namespace beemaster {
    template <class count_t>
    class IncrementMergeOperator : public rocksdb::AssociativeMergeOperator {
        public:
            /// Gives the client a way to express the read -> modify -> write semantics
            ///
            /// key:           (IN) The key that's associated with this merge operation.
            /// existing_value:(IN) null indicates the key does not exist before this op
            /// value:         (IN) the value to update/merge the existing_value with
            /// new_value:    (OUT) Client is responsible for filling the merge result
            ///                     here. The string that new_value is pointing
            ///                     to will be empty.
            /// logger:        (IN) Client could use this to log errors during merge.
            ///
            /// Return true on success.
            /// All values passed in will be client-specific values. So if this method
            /// returns false, it is because client specified bad data or there was
            /// internal corruption. The client should assume that this will be treated
            /// as an error by the library
// gcc.gnu.org/onlinedocs/gcc/Diagnostic-Pragmas.html
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
            virtual bool Merge(const rocksdb::Slice& key,
                               const rocksdb::Slice* existing_value,
                               const rocksdb::Slice& value,
                               std::string* new_value,
                               rocksdb::Logger* logger) const override {
#pragma GCC diagnostic pop
                count_t count_e = existing_value != nullptr ? *(count_t*)existing_value->data() : 0;
                count_t count_n = *(count_t*)value.data();
                count_t val = count_e + count_n;
                new_value->append((char*)&val, sizeof(val));
                return true;
            }

            virtual const char* Name() const override { return "IncrementMergeOperator"; }
    };

    template <class count_t>
    RocksStorage<count_t>::RocksStorage(std::string db_name)
        : acu::Storage::Storage(db_name) {
            Options.create_if_missing = true;
            Options.merge_operator.reset(new IncrementMergeOperator<count_t>);
            // TODO what about unified read/writeOptions?
            rocksdb::Status status = rocksdb::DB::Open(Options, db_name, &Database);
            assert(status.ok());
        }

    // Gracefully closes DB
    template <class count_t>
    RocksStorage<count_t>::~RocksStorage() {
        delete Database;
    }

    template <class count_t>
    void RocksStorage<count_t>::Persist(const acu::IncomingAlert *alert) {
        assert(alert);
        // noop
    }

    template <class count_t>
    bool RocksStorage<count_t>::Set(const std::string key, const count_t value){
        rocksdb::Status status = Database->Put(rocksdb::WriteOptions(), key, rocksdb::Slice((char*)&value, sizeof(value)));
        return status.ok();
    }

    template <class count_t>
    bool RocksStorage<count_t>::Increment(const std::string key, const count_t value) {
        rocksdb::Status status = Database->Merge(rocksdb::WriteOptions(), key, rocksdb::Slice((char*)&value, sizeof(value)));
        return status.ok();
    }

    template <class count_t>
    count_t RocksStorage<count_t>::Get(const std::string key) {
        std::string str = "";
        Database->Get(rocksdb::ReadOptions(), key, &str);
        return *((count_t*)str.data());
    }

    template<class count_t>
    rocksdb::Iterator* RocksStorage<count_t>::GetIterator() {
        return Database->NewIterator(rocksdb::ReadOptions());
    }
}
