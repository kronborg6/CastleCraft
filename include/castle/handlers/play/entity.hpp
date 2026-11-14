#pragma once

#include "castle/client.hpp"
#include "castle/handlers/play/play.hpp"
namespace castle::handlers::play {
  PlayPacketWriter addPlayerEntity(const Client& client);

  void moveEntity(Client& client,
                  const player::Position& oldPos,
                  const player::Position& newPos,
                  bool onGround);
} // namespace castle::handlers::play
