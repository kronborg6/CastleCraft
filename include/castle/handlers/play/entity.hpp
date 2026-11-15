#pragma once

#include "castle/client.hpp"
#include "castle/handlers/play/play.hpp"
#include "castle/player/postion.hpp"
namespace castle::handlers::play {
PlayPacketWriter addPlayerEntity(const Client& client);

void moveEntity(Client& client,
                const player::Position& oldPos,
                const player::Position& newPos,
                bool onGround,
                player::UpdateType type);

PlayPacketWriter removeEntitys(std::span<const std::int32_t> ids);
} // namespace castle::handlers::play
