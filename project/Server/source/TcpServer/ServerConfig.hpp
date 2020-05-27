//
// Created by paul_s on 05.05.2020.
//

#pragma once
#include <string>

struct ServerConfig {

  ServerConfig() :
      _ip("0.0.0.0"),
      _port(8008),
      _listenCount(1024),
      _workerThreadsCount(2),
      _queueWorkersCount(1),
      _delim("delim") {}

  ServerConfig(const std::string &ipAddress,
               unsigned int port,
               int listenCount,
               unsigned int workerThreadsCount,
               unsigned int queueWorkersCount,
               const std::string &delim) :
      _ip(ipAddress),
      _port(port),
      _listenCount(listenCount),
      _workerThreadsCount(workerThreadsCount),
      _queueWorkersCount(queueWorkersCount),
      _delim(delim) {}

  std::string _ip;
  unsigned int _port;
  int _listenCount;
  unsigned int _workerThreadsCount;
  unsigned int _queueWorkersCount;
  std::string _delim;

};
