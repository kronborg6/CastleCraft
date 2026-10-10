#pragma once

#include "../client.hpp"

namespace minecraft::protocol::handlers {
  void handleLogin(Client& client);
  void handleLoginRequst(Client& client);
  // void handlePingRequst(Client& client);
} // namespace minecraft::protocol::handlers
