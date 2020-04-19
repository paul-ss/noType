#pragma once

#include <fstream>

#include <SFML/System.hpp>

class FileLogger : public sf::NonCopyable {
    friend FileLogger& operator<<(FileLogger &logger, const logType l_type);
    friend FileLogger& operator<<(FileLogger& logger, const char* text);

    public:
        enum class logType {LOG_ERROR, LOG_WARNING, LOG_INFO};
        explicit FileLogger (const char *fname = "noType_Log.txt");
        ~FileLogger ();

    private:
        std::ofstream myFile;
        unsigned int numWarnings;
        unsigned int numErrors;
};
