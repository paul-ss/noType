#include "Logger.hpp"

#define LOG_FILE "/var/log/notype/server.log"

void initLogger(boost::log::trivial::severity_level l_severity) {
    boost::log::register_simple_formatter_factory<boost::log::trivial::severity_level, char>("Severity");

    boost::log::add_file_log(
        boost::log::keywords::file_name = LOG_FILE,
        boost::log::keywords::format = "[%TimeStamp%] [%ThreadID%] [%Severity%] %Message%"
    );

    boost::log::core::get()->set_filter
    (
        boost::log::trivial::severity >= l_severity
    );

    boost::log::add_common_attributes();
}

