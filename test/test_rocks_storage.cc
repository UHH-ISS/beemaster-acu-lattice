/* test_rocks_storage.h
 * ACU Implementation (test)
 *
 * Test functionality of RocksStorage
 *
 * @author: 1jost
 */

#include "catch.hpp"

#include <rocks_storage.h>

TEST_CASE("Testing RocksDB initialisation", "[rocks_storage]") {
    // TODO FIXME XXX DIRTY AF
    std::system("rm -rf /tmp/test_storage");
    auto storage = new beemaster::RocksStorage("/tmp/test_storage");

    delete storage;
}
