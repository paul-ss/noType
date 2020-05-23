//
// Created by paul_s on 23.05.2020.
//

#include "ProgramOptionsParser.hpp"

ProgramOptionsParser::ProgramOptionsParser() :
    _desc{"Options"} {
  _desc.add_options()
      ("help,h", "Help screen")
      ("trace","TRACE")
      ("debug","DEBUG")
      ("info", "INFO")
      ("warning","WARNING")
      ("error","ERROR")
      ("fatal", "FATAL")
      ("stdout", "TRACE logging to stdout))")
      (
          "config,c",
          po::value<std::string>()->default_value("config/Server.conf"),
          "Specify config file path."
      );
}


bool ProgramOptionsParser::parseProgOptions(int argc, const char *argv[]) {
  try {
    store(parse_command_line(argc, argv, _desc), _vm);
    notify(_vm);

    _parsedData.configFilePath = _vm["config"].as<std::string>();


    if (_vm.count("help")) {
      std::cout << _desc << std::endl;
      return false;
    } else if (_vm.count("trace")) {
      _parsedData.loggerSecurityLevel = boost::log::trivial::trace;
    } else if (_vm.count("debug")) {
      _parsedData.loggerSecurityLevel = boost::log::trivial::debug;
    } else if (_vm.count("info")) {
      _parsedData.loggerSecurityLevel = boost::log::trivial::info;
    } else if (_vm.count("warning")) {
      _parsedData.loggerSecurityLevel = boost::log::trivial::warning;
    } else if (_vm.count("error")) {
      _parsedData.loggerSecurityLevel = boost::log::trivial::error;
    } else if (_vm.count("fatal")) {
      _parsedData.loggerSecurityLevel = boost::log::trivial::fatal;
    } else if (_vm.count("stdout")) {
      _parsedData.stdoutLog = true;
    } else {
      std::cout << _desc << std::endl;
      return false;
    }


  } catch (const po::error &ex) {
    std::cerr << ex.what() << std::endl;
    return false;

  } catch (...) {
    std::cerr << "Unknown exception" << std::endl;
    return false;
  }

  return true;
}


ParsedData &&ProgramOptionsParser::extractData() {
  return std::move(_parsedData);
}