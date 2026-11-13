#pragma once

#include "castleProtocol/package/readPackage.hpp"
#include "castleProtocol/package/writePacket.hpp"
#include "castleProtocol/protocol/play.hpp"
namespace castle::handlers::play {
  using PlayPacketWriter = castle::protocol::PacketWriter<castle::protocol::PlayClientbound>;
  using PlayPacketReader = castle::protocol::PacketReader<castle::protocol::PlayServerbound>;
} // namespace castle::handlers::play
