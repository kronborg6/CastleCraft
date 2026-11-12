#pragma once

#include "castle/client.hpp"
#include "castle/server/server.hpp"

namespace castle::handlers {
  void handleConfiguration(server::Server& server, Client& client);
}
