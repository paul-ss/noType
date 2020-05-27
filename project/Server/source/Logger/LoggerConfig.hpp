//
// Created by paul_s on 23.05.2020.
//

#pragma once

#include <string>

struct LoggerConfig {
  LoggerConfig() :
      _logFilePath("/var/log/notype/server.log") {}
  explicit LoggerConfig(const std::string &logFilePath) :
      _logFilePath(logFilePath) {}

public:
  std::string _logFilePath;
};