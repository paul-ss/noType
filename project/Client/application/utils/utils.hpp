#pragma once

#include <iostream>
#include <sstream>
#include <algorithm>
#include <unistd.h>

namespace utils {
    /*inline std::string GetWorkingDirectory() {
        char cwd[1024];
        if(getcwd(cwd, sizeof(cwd)) != nullptr) {
            return std::string(cwd) + std::string("/");
        }
        return "";
    }*/

    inline void ReadQuotedString(std::stringstream& stream,
        std::string& string) {
        stream >> string;
        if (string.at(0) == '"') {
            while (string.at(string.length() - 1) != '"' || !stream.eof()) {
                std::string str;
                stream >> str;
                string.append(" " + str);
            }
        }
        string.erase(std::remove(string.begin(), string.end(), '"'), string.end());
    }

}  // namespace utils
