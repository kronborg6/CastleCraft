#pragma once

#include "castle/client.hpp"

namespace castle::handlers {
  void handleStatus(Client& client);
  void handleStatusRequst(Client& client);
  void handlePingRequst(Client& client);
} // namespace castle::handlers
