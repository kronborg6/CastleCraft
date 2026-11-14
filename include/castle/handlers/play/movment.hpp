#pragma once

#include "castle/client.hpp"
#include "castle/handlers/play/play.hpp"
#include "castle/player/postion.hpp"
namespace castle::handlers::play {
  void SetPlayerPositionAndRotation(PlayPacketReader& recive, Client& client);
  void SetPlayerPosition(PlayPacketReader& recive, Client& client);
  void SetPlayerRotation(PlayPacketReader& recive, Client& client);

  void teleportPlayer(Client& client, player::Position postion);
} // namespace castle::handlers::play
