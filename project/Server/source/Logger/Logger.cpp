#include "Logger.hpp"

#include <mutex>

namespace Logger {

std::mutex loggerMutex;

void initLogger(boost::log::trivial::severity_level l_severity,
                const LoggerConfig &config) {
    std::unique_lock<std::mutex> lock(loggerMutex);
    boost::log::register_simple_formatter_factory<boost::log::trivial::severity_level, char>("Severity");

    boost::log::add_file_log(
        boost::log::keywords::auto_flush = true,
        boost::log::keywords::file_name = config._logFilePath,
        boost::log::keywords::format = "[%TimeStamp%] [%ThreadID%] [%Severity%] %Message%"
    );

    boost::log::core::get()->set_filter
    (
        boost::log::trivial::severity >= l_severity
    );

    boost::log::add_common_attributes();
}

} // Logger