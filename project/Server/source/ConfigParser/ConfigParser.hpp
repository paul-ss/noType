//
// Created by paul_s on 23.05.2020.
//

#pragma once

#include "RoomConfig.hpp"
#include "ServerConfig.hpp"
#include "LoggerConfig.hpp"

#include <string>


#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>


#define IP_ADDRESS_PATH "ipAddress"
#define PORT_PATH "port"
#define LISTEN_COUNT_PATH "listenCount"
#define WORKER_THREADS_COUNT_PATH "workerThreadsCount"
#define QUEUE_WORKERS_COUNT_PATH "queueWorkersCount"
#define DELIM_PATH "delim"

#define FINISH_LINE_PATH "finishLine"
#define MAX_PLAYERS_COUNT_PATH "maxPlayersCount"
#define WAIT_DURATION_PATH "waitDuration"
#define PLAY_DURATION_PATH "playDuration"
#define PLAY_HANDLE_INTERVAL_PATH "playHandleInterval"
#define END_DURATION_PATH "endDuration"

#define LOG_FILE_PATH "logFile"



namespace pt = boost::property_tree;


class ConfigParser {
public:
  void parseConfig(const std::string &configPath);
  RoomConfig &&extractRoomConfig();
  ServerConfig &&extractServerConfig();
  LoggerConfig getLoggerConfig();

private:
  void parseRoomConfig(const pt::ptree &pTree);
  void parseTcpServerConfig(const pt::ptree &pTree);
  void parseLoggerConfig(const pt::ptree &pTree);

private:
  std::unique_ptr<RoomConfig> _roomConfig;
  std::unique_ptr<ServerConfig> _serverConfig;
  std::unique_ptr<LoggerConfig> _loggerConfig;
};

