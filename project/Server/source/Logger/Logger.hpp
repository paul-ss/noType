#pragma once

#include "LoggerConfig.hpp"

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>

namespace Logger {

void initLogger(boost::log::trivial::severity_level l_severity,
                const LoggerConfig &config);

} // Logger