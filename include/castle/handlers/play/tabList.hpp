#pragma once

#include "castle/client.hpp"
#include "castle/handlers/play/play.hpp"
namespace castle::handlers::play {
  PlayPacketWriter playerInfoUpdate(Client& client, player::PlayerAction action);

}
