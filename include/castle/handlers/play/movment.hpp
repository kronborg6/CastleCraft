#pragma once

#include "castle/client.hpp"
#include "castle/handlers/play/play.hpp"
namespace castle::handlers::play {
  void SetPlayerPositionAndRotation(PacketPlayServer& recive, Client& client);
  void SetPlayerPosition(PacketPlayServer& recive, Client& client);
  void SetPlayerRotation(PacketPlayServer& recive, Client& client);
} // namespace castle::handlers::play
