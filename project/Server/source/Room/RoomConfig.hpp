//
// Created by paul_s on 25.04.2020.
//

#pragma once
#include <iostream>

struct RoomConfig {
  RoomConfig() :
      _finishLine(200),
      _maxPlayersCount(5),
      _waitDuration(5000),
      _playDuration(7000),
      _playHandlerInterval(3000),
      _endDuration(3000) {}

  size_t _finishLine; // n symbols
  size_t _maxPlayersCount;
  size_t _waitDuration; // milliseconds
  size_t _playDuration; // milliseconds
  size_t _playHandlerInterval;  // milliseconds
  size_t _endDuration;  // milliseconds
};
