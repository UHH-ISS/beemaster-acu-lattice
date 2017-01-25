/* config_parser.cc
 * ACU Implementation
 *
 * <config_parser.h>
 * // based on www.adp-gmbh.ch/cpp/config_file.html
 */
#include "config_parser.h"
#include <fstream>
#include <cstdlib>

namespace beemaster {
    std::string trim(std::string const& source) {
        char const* delims = " \t\r\n";
        std::string result(source);
        std::string::size_type index = result.find_last_not_of(delims);
        if (index != std::string::npos)
            result.erase(++index);

        index = result.find_first_not_of(delims);
        if (index != std::string::npos)
            result.erase(0, index);
        else
            result.erase();
        return result;
    }

    ConfigParser::ConfigParser(std::string const &filepath) {
        std::ifstream file(filepath.c_str());
        std::string line, name, value, section;
        size_t equal_pos;

        while (std::getline(file, line)) {
            // empty or commented line
            if (!line.length() || line[0] == '#')
                continue;
            // section
            if (line[0] == '[') {
                section = trim(line.substr(1, line.find(']')-1));
                continue;
            }
            // value
            equal_pos = line.find('=');
            name = trim(line.substr(0, equal_pos));
            value = trim(line.substr(equal_pos+1));

            values[section + '/' + name] = value;
        }
        file.close();
    }

    std::string ConfigParser::GetString(std::string const &section, std::string const &entry) {
        std::string key = section + '/' + entry;
        if (!values.count(key))
            throw std::runtime_error("Key '" + key + "' does not exist.");
        return values[key];
    }

    int ConfigParser::GetInt(std::string const &section, std::string const &entry) {
        std::string key = section + '/' + entry;
        if (!values.count(key))
            throw std::runtime_error("Key '" + key + "' does not exist.");
        return atoi(values[key].c_str());
    }
}
