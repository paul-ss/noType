#include "fileLogger.hpp"
#include "exceptions.hpp"

FileLogger::FileLogger (const char *fname = "noType_Log.txt") : 
        numWarnings (0U),
        numErrors (0U) {

    myFile.open(fname);
    if (!myFile.is_open()) {
        throw InvalidFile();
    }
}

FileLogger::~FileLogger () {
    if (myFile.is_open()) {
        myFile << std::endl << std::endl;
        myFile << numWarnings << " warnings" << std::endl;
        myFile << numErrors << " errors" << std::endl;
        myFile.close();
    } else {
        throw InvalidFile();
    }
}

FileLogger& operator<<(FileLogger &logger, const logType type) {
    switch (type) {
        case FileLogger::logType::LOG_ERROR:
            logger.myFile << "[ERROR]: ";
            ++logger.numErrors;
            break;

        case FileLogger::logType::LOG_WARNING:
            logger.myFile << "[WARNING]: ";
            ++logger.numWarnings;
            break;

        default:
            logger.myFile << "[INFO]: ";
            break;
    }
    return logger;
}

FileLogger& operator<<(FileLogger& logger, const char* text) {
    logger.myFile << text << std::endl;
    return logger;
}
