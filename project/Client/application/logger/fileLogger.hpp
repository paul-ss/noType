#pragma once

#include <fstream>

#include <SFML/System.hpp>

enum class logType {ERROR, WARNING, INFO};

class FileLogger : public sf::NonCopyable {
    public:
        static FileLogger* getInstance();
        void writeToLog(const logType type, const char* msg);

    protected:
        explicit FileLogger(const char* fName = "noType_Log.txt");
        ~FileLogger();

    private:
        static FileLogger* _pInstance;
        static std::mutex _mutex;

    private:
        std::ofstream _myFile;
        unsigned int _numWarnings;
        unsigned int _numErrors;
};
