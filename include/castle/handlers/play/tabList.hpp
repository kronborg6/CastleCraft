#pragma once

#include "castle/client.hpp"
#include "castle/handlers/play/play.hpp"

namespace castle::handlers::play {
PlayPacketWriter playerInfoUpdate(Client& client, player::PlayerAction action);

PlayPacketWriter playerInfoRemove(std::span<const std::array<uint8_t, 16>> uuids);

} // namespace castle::handlers::play
