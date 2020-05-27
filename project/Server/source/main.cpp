//
// Created by paul_s on 15.04.2020.
//


#include "Setup.hpp"
#include "Logger.hpp"
#include "ProgramOptionsParser.hpp"

namespace po = boost::program_options;



int main(int argc, const char *argv[]) {
  ProgramOptionsParser poParser;
  if (!poParser.parseProgOptions(argc, argv)) {
    return EXIT_SUCCESS;
  }

  auto parsedOptions = poParser.extractData();

  Setup serverSetup;
  serverSetup.parseConfig(parsedOptions.configFilePath);

  if (!parsedOptions.stdoutLog) {
    serverSetup.initLogger(parsedOptions.loggerSecurityLevel);
  }

  serverSetup.setup();
  serverSetup.start();

  return 0;
}
