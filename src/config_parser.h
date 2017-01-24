/* config_parser.h
 * ACU Implementation
 *
 * Provide access to configuration files.
 *
 * @author: 1jost
 */

#ifndef ACU_IMPL_CONFIG_PARSER_H_H
#define ACU_IMPL_CONFIG_PARSER_H_H

#include <unordered_map>

namespace beemaster {
    class ConfigParser {
    public:
        /// Read *filepath* as an INI-File.
        ///
        /// Section are introduced with `[header]`, comments with `#` (as first character of line)
        /// and key-values as `key = value`.
        ///
        /// @param filepath     The file to read
        ConfigParser(std::string const& filepath);

        /// Get string for given path.
        ///
        /// @param section  The section to look in
        /// @param entry    The entry to look for
        /// @return         The stored value
        std::string GetString(std::string const& section, std::string const& entry);

        /// Get interger for given path.
        ///
        /// @param section  The section to look in
        /// @param entry    The entry to look for
        /// @return         The stored value (atio).
        int GetInt(std::string const& section, std::string const& entry);
    private:
        std::unordered_map<std::string, std::string> values;
    };
}

#endif //ACU_IMPL_CONFIG_PARSER_H_H
