/* test_vector_storage.h
 * ACU Implementation (test)
 *
 * Test functionality of VectorStorage
 *
 * @author: 1jost
 */

#include "catch.hpp"

#include <lattice_incoming_alert.h>
#include <vector_storage.h>

TEST_CASE("Testing VectorStorage initialisation", "[vector_storage]") {
    auto storage = new beemaster::VectorStorage("test_db");
    delete storage;
}

TEST_CASE("Testing read/write operations", "[vector_storage]") {
    auto storage = new beemaster::VectorStorage("test_db");

    // create alert
    // - topics:
    auto topic1 = new std::string("Topic 1");
    auto topic2 = new std::string("Topic 2");
    // - message:
    auto time_stamp = std::chrono::system_clock::now();
    auto val = std::chrono::duration_cast<std::chrono::duration<double>>(time_stamp.time_since_epoch());
    auto broker_stamp = broker::time_point{val.count()};
    auto rec = broker::record({
        broker::record::field(broker_stamp),
        broker::record::field("127.0.0.1"),
        broker::record::field((acu::port_t)8080),
        broker::record::field("127.0.0.1"),
        broker::record::field((acu::port_t)9090),
    });
    auto message = broker::message{"",rec, "TCP"};

    auto alert = new beemaster::LatticeIncomingAlert(topic1, message);
    auto alert2 = new beemaster::LatticeIncomingAlert(topic2, message);
    auto alert3 = new beemaster::LatticeIncomingAlert(topic1, message);
    SECTION("Single insert") {
        // INSERT
        REQUIRE_NOTHROW(storage->Persist(alert2));

        // POP
        auto popped = storage->Pop(*topic2);
        REQUIRE(popped->size() == 1);
        REQUIRE(popped->at(0) == alert2);
        delete popped;
        popped = storage->Pop(*topic1);
        REQUIRE(popped->size() == 0);
        delete popped;
        popped = storage->Pop(*topic2);
        REQUIRE(popped->size() == 0);
        delete popped;
    }

    SECTION("Multiple inserts") {
        // MULTIPLE INSERT
        REQUIRE_NOTHROW(storage->Persist(alert));
        REQUIRE_NOTHROW(storage->Persist(alert2));
        REQUIRE_NOTHROW(storage->Persist(alert3));

        auto popped = storage->Pop(*topic1);
        REQUIRE(popped->size() == 2);
        REQUIRE(popped->at(0) == alert);
        REQUIRE(popped->at(1) == alert3);
        delete popped;
        popped = storage->Pop(*topic1);
        REQUIRE(popped->size() == 0);
        delete popped;
        popped = storage->Pop(*topic1);
        REQUIRE(popped->size() == 0);
        delete popped;
    }
    // CLEANUP
    delete storage;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdelete-non-virtual-dtor"
    // free should work too (removes warnings), but seems to leave even more memory.
    delete alert;
    delete alert2;
    delete alert3;
#pragma GCC diagnostic pop
}
