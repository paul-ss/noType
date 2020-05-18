#include "logger.hpp"

#define LOG_FILE "logNoType.txt"

void init_logger() {
    boost::log::register_simple_formatter_factory<boost::log::trivial::severity_level, char>("Severity");

    boost::log::add_file_log(
        boost::log::keywords::file_name = LOG_FILE,
        boost::log::keywords::format = "[%TimeStamp%] [%ThreadID%] [%Severity%] %Message%"
    );

    boost::log::core::get()->set_filter
    (
        boost::log::trivial::severity >= boost::log::trivial::info
    );

    boost::log::add_common_attributes();
}

