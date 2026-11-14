#pragma once

#include "castle/handlers/play/play.hpp"
#include "castle/nbt.hpp"
#include "castle/network/tcp_connection.hpp"
#include <cstdint>
namespace castle::handlers::play {
  enum class GameEvent : std::uint8_t {
    NoRespawnBlockAvailable = 0x00,
    BeginRaining = 0x01,
    EndRaining = 0x02,
    ChangeGameMode = 0x03,
    WinGame = 0x04,
    DemoEvent = 0x05,
    PlayArrowHitSound = 0x06,
    ChangeRainLevel = 0x07,
    ChangeThunderLevel = 0x08,
    PlayPufferfishStingSound = 0x09,
    PlayElderGuardianJumpScare = 0x0A,
    ToggleImmediateRespawn = 0x0B,
    ToggleLimitedCrafting = 0x0C,
    StartWaitingForChunks = 0x0D
  };
  void gameEvent(castle::network::TcpConnection& connection, GameEvent event, float value);

  PlayPacketWriter systemMessage(const nbt::Nbt& message, bool type);
} // namespace castle::handlers::play
