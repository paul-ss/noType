//
// Created by paul_s on 17.04.2020.
//

#pragma once
#include <iostream>

class Command {
public:
  Command(const std::string &data, const std::string &connectionUUID):
      connectionUUID(connectionUUID),
      data(data) {}

  Command() = default;
  virtual ~Command() = default;

  virtual std::string parseToJSON () {
    return data;
  }
  std::string connectionUUID;
  std::string data;
};
