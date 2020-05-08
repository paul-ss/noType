#pragma once

#include <iostream>
#include <sstream>
#include <algorithm>
#include <unistd.h>

namespace utils {
    inline void ReadQuotedString(std::stringstream& l_stream, std::string& l_string) {
        l_stream >> l_string;
        if (l_string.at(0) == '"') {
            while (l_string.at(l_string.length() - 1) != '"' || !l_stream.eof()) {
                std::string str;
                l_stream >> str;
                l_string.append(" " + str);
            }
        }
        l_string.erase(std::remove(l_string.begin(), l_string.end(), '"'), l_string.end());
    }
}  // namespace utils
