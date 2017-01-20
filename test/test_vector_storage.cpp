/* test_vector_storage.h
 * ACU Implementation (test)
 *
 * Test functionality of VectorStorage
 *
 * @author: 1jost
 */

#include "catch.hpp"

#include <vector_storage.h>

TEST_CASE("Testing VectorStorage initialisation", "[vector_storage]") {
    auto storage = new beemaster::VectorStorage("test_db");
    delete storage;
}

TEST_CASE("Testing read/write operations", "[vector_storage]") {
    auto storage = new beemaster::VectorStorage("test_db");

    // create alert
    // - topics:
    std::string topic1 = "Topic 1";
    std::string topic2 = "Topic 2";
    // - message:
    std::vector<broker::data> message {
            0,
            broker::data("incident_type"),
            broker::data("protocol"),
            broker::data("sourceIP"),
            1337,
            broker::data("destinationIP"),
            8080
    };
    auto alert = new acu::IncomingAlert(&topic1, message);
    auto alert2 = new acu::IncomingAlert(&topic2, message);
    auto alert3 = new acu::IncomingAlert(&topic1, message);

    SECTION("Single insert") {
        // INSERT
        REQUIRE_NOTHROW(storage->Persist(alert2));

        // POP
        auto popped = storage->Pop(topic2);
        REQUIRE(popped->size() == 1);
        REQUIRE(popped->at(0) == alert2);
        delete popped;
        popped = storage->Pop(topic1);
        REQUIRE(popped->size() == 0);
        delete popped;
        popped = storage->Pop(topic2);
        REQUIRE(popped->size() == 0);
        delete popped;
    }

    SECTION("Multiple inserts") {
        // MULTIPLE INSERT
        REQUIRE_NOTHROW(storage->Persist(alert));
        REQUIRE_NOTHROW(storage->Persist(alert2));
        REQUIRE_NOTHROW(storage->Persist(alert3));

        auto popped = storage->Pop(topic1);
        REQUIRE(popped->size() == 2);
        REQUIRE(popped->at(0) == alert);
        REQUIRE(popped->at(1) == alert3);
        delete popped;
        popped = storage->Pop(topic1);
        REQUIRE(popped->size() == 0);
        delete popped;
        popped = storage->Pop(topic1);
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
