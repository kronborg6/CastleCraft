#pragma once

#include "castle/client.hpp"
#include "castle/handlers/play/play.hpp"
namespace castle::handlers::play {
  PlayPacketWriter addPlayerEntity(const Client& client);
}
