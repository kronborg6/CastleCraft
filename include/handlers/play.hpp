#pragma once

#include "protocol/protocol.hpp"

namespace minecraft::protocol::handlers {
  void handlePlayState(Client& client);
  void initialiizePlay(Client& clinet);
} // namespace minecraft::protocol::handlers
