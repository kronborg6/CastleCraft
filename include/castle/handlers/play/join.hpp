#pragma once
#include "castle/client.hpp"
#include "castle/server/server.hpp"
namespace castle::handlers::play {
  void join(server::Server&, Client& client);
  void leave(server::Server&, Client& client);
} // namespace castle::handlers::play
