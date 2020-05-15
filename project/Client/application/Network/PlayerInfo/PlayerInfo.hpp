#pragma once

#include <string>

struct PlayerInfo {
  std::string id;
  std::string name;
  std::uint16_t position;
  double speed;
  enum class Status {
    Player,
    Winner,
    Lost
  };
  Status status;
};
