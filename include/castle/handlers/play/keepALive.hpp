#pragma once

#include "castle/client.hpp"
#include "castle/handlers/play/play.hpp"
namespace castle::handlers::play {

  void keepALive(PlayPacketReader packet, Client& client);
} // namespace castle::handlers::play
