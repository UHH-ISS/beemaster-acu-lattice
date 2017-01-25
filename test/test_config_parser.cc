/* test_config_parser.cc
 * ACU Implementation (test)
 *
 * Test functionality of ConfigParser
 *
 * @author: 1jost
 */

#include "catch.hpp"
#include <config_parser.h>
#include <fstream>

TEST_CASE("Testing complete behaviour of ConfigParser", "[config_parser]") {
    auto path = "/tmp/test_config_parser.ini";
    // TODO FIXME hacky hacky...
    system((std::string("rm ") + path).c_str());
    std::ofstream file;
    file.open(path);
    file << "[receiver]\n";
    file << "address = localhost\n";
    file << "port = 1337\n\n";
    file << "[sender]\n";
    file << "address = 127.0.0.1\n";
    file << "port = 42\n";
    file.close();

    beemaster::ConfigParser parser(path);

    REQUIRE(parser.GetString("receiver", "address") == "localhost");
    REQUIRE(parser.GetInt("receiver", "port") == 1337);
    REQUIRE(parser.GetInt("sender", "port") == 42);
}