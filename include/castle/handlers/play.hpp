#pragma once

#include "castle/client.hpp"
#include "castleProtocol/package/writePacket.hpp"
#include "castleProtocol/protocol/play.hpp"

namespace castle::handlers {
  using PlayPacketWriter = castle::protocol::PacketWriter<castle::protocol::PlayClientbound>;
  void handlePlayState(Client& client);
  void initialiizePlay(Client& clinet);
} // namespace castle::handlers
