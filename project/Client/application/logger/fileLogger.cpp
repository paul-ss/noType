#include "fileLogger.hpp"
#include "exceptions.hpp"

FileLogger::FileLogger(const char *fname = "noType_Log.txt") :
        _numWarnings(0U),
        _numErrors(0U) {

    _myFile.open(fname);
    if (!_myFile.is_open()) {
        throw InvalidFile();
    }
}

FileLogger::~FileLogger () {
    if (_myFile.is_open()) {
        _myFile << std::endl << std::endl;
        _myFile << _numWarnings << " warnings" << std::endl;
        _myFile << _numErrors << " errors" << std::endl;
        _myFile.close();
    }
}

void FileLogger::writeToLog(const logType type, const char* msg) {
    switch (type) {
        case logType::ERROR: {
            _myFile << "[ERROR]: " << msg << std::endl;
            ++_numErrors;
            break;
        }

        case logType::WARNING: {
            _myFile << "[WARNING]: " << msg << std::endl;
            ++_numWarnings;
            break;
        }

        default: {
            _myFile << "[INFO]: " << msg << std::endl;
            break;
        }
    }
}

FileLogger* FileLogger::getInstance() {
    if (_pInstance == nullptr) {
        std::lock_guard<std::mutex> lock(_mutex);

        if (_pInstance == nullptr) {
            FileLogger* _pInstance = new FileLogger();
        }
    }
    return _pInstance;
}
