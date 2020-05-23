//
// Created by paul_s on 23.05.2020.
//

#pragma once

#include "RoomConfig.hpp"
#include "ServerConfig.hpp"

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



namespace pt = boost::property_tree;


class ConfigParser {
public:
  void parseConfig(const std::string &configPath);
  RoomConfig &&extractRoomConfig();
  ServerConfig &&extractServerConfig();

private:
  void parseRoomConfig(const std::string &configPath);
  void parseTcpServerConfig(const std::string &configPath);

private:
  std::unique_ptr<RoomConfig> _roomConfig;
  std::unique_ptr<ServerConfig> _serverConfig;
};

