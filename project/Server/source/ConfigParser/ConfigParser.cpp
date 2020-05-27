//
// Created by paul_s on 23.05.2020.
//

#include "ConfigParser.hpp"
#include "Logger.hpp"






void ConfigParser::parseConfig(const std::string &configPath) {
  pt::ptree pTree;

  try {
    pt::read_json(configPath, pTree);
  } catch (const pt::ptree_error &exc) {
    BOOST_LOG_TRIVIAL(fatal) << "parseConfig : exception : " << exc.what();

  } catch (...) {
    BOOST_LOG_TRIVIAL(fatal) << "parseConfig : unknown exception while reading config";
  }



  try {
    parseTcpServerConfig(pTree);
  } catch (const pt::ptree_error &exc) {
    BOOST_LOG_TRIVIAL(fatal) << "WARNING! Server config was not applied. Taken default values.\n"
                             << "parseConfig : exception : " << exc.what();
    _serverConfig = std::make_unique<ServerConfig>();

  } catch (...) {
    BOOST_LOG_TRIVIAL(fatal) << "WARNING! Server config was not applied. Taken default values.\n"
                             << "parseConfig : unknown exception";
    _serverConfig = std::make_unique<ServerConfig>();

  }


  try {
    parseRoomConfig(pTree);
  } catch (const pt::ptree_error &exc) {
    BOOST_LOG_TRIVIAL(fatal) << "WARNING! Room config was not applied. Taken default values.\n"
                             << "parseConfig : exception : " << exc.what();
    _roomConfig = std::make_unique<RoomConfig>();

  } catch (...) {
    BOOST_LOG_TRIVIAL(fatal) << "WARNING! Room config was not applied. Taken default values.\n"
                             << "parseConfig : unknown exception";
    _roomConfig = std::make_unique<RoomConfig>();

  }

  try {
    parseLoggerConfig(pTree);
  } catch (const pt::ptree_error &exc) {
    BOOST_LOG_TRIVIAL(fatal) << "WARNING! Logger config was not applied. Taken default values.\n"
                             << "parseConfig : exception : " << exc.what();
    _loggerConfig = std::make_unique<LoggerConfig>();

  } catch (...) {
    BOOST_LOG_TRIVIAL(fatal) << "WARNING! Logger config was not applied. Taken default values.\n"
                             << "parseConfig : unknown exception";
    _loggerConfig = std::make_unique<LoggerConfig>();

  }
}


RoomConfig &&ConfigParser::extractRoomConfig() {
  if (_roomConfig == nullptr) {
    _roomConfig = std::make_unique<RoomConfig>();
    BOOST_LOG_TRIVIAL(fatal) << "WARNING! Room config was not applied. Taken default values.";
  }

  return(std::move(*_roomConfig.release()));
}


ServerConfig &&ConfigParser::extractServerConfig() {
  if (_serverConfig == nullptr) {
    _serverConfig = std::make_unique<ServerConfig>();
    BOOST_LOG_TRIVIAL(fatal) << "WARNING! Server config was not applied. Taken default values.";
  }

  return(std::move(*_serverConfig.release()));
}


LoggerConfig ConfigParser::getLoggerConfig() {
  if (_loggerConfig == nullptr) {
    _loggerConfig = std::make_unique<LoggerConfig>();
    BOOST_LOG_TRIVIAL(fatal) << "WARNING! Logger config was not applied. Taken default values.";
  }

  return *_loggerConfig.release();
}




void ConfigParser::parseRoomConfig(const pt::ptree &pTree) {
  _roomConfig = std::make_unique<RoomConfig>(
      pTree.get<size_t>(FINISH_LINE_PATH),
      pTree.get<size_t>(MAX_PLAYERS_COUNT_PATH),
      pTree.get<size_t>(WAIT_DURATION_PATH),
      pTree.get<size_t>(PLAY_DURATION_PATH),
      pTree.get<size_t>(PLAY_HANDLE_INTERVAL_PATH),
      pTree.get<size_t>(END_DURATION_PATH)
      );
}



void ConfigParser::parseTcpServerConfig(const pt::ptree &pTree) {
  _serverConfig = std::make_unique<ServerConfig>(
      pTree.get<std::string>(IP_ADDRESS_PATH),
      pTree.get<unsigned int>(PORT_PATH),
      pTree.get<int>(LISTEN_COUNT_PATH),
      pTree.get<unsigned int>(WORKER_THREADS_COUNT_PATH),
      pTree.get<unsigned int>(QUEUE_WORKERS_COUNT_PATH),
      pTree.get<std::string>(DELIM_PATH)
  );
}

void ConfigParser::parseLoggerConfig(const pt::ptree &pTree) {
  _loggerConfig = std::make_unique<LoggerConfig>(
      pTree.get<std::string>(LOG_FILE_PATH)
  );
}