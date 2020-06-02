#include "logger.hpp"

#include <mutex>

#define LOG_FILE "logNoType.txt"

std::mutex mtx;

void init_logger() {
    std::unique_lock<std::mutex> lock(mtx);
    boost::log::register_simple_formatter_factory<boost::log::trivial::severity_level, char>("Severity");

    boost::log::add_file_log(
        boost::log::keywords::auto_flush = true,
        boost::log::keywords::file_name = LOG_FILE,
        boost::log::keywords::format = "[%TimeStamp%] [%ThreadID%] [%Severity%] %Message%"
    );

    boost::log::core::get()->set_filter
    (
        boost::log::trivial::severity >= boost::log::trivial::info
    );

    boost::log::add_common_attributes();
}

