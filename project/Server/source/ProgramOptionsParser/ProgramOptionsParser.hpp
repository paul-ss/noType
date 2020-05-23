//
// Created by paul_s on 23.05.2020.
//

#pragma once

#include "Logger.hpp"

#include <boost/program_options.hpp>
#include <optional>

namespace po = boost::program_options;


struct ParsedData {
  ParsedData() :
      loggerSecurityLevel(boost::log::trivial::info),
      stdoutLog(false) {}

public:
  boost::log::trivial::severity_level loggerSecurityLevel;
  bool stdoutLog;
  std::string configFilePath;
};



class ProgramOptionsParser {
public:
  ProgramOptionsParser();
  bool parseProgOptions(int argc, const char *argv[]);
  ParsedData &&extractData();

private:
  po::options_description _desc;
  po::variables_map _vm;
  ParsedData _parsedData;
};


