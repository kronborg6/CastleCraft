#include "castle/handlers/play/gameEvent.hpp"
#include "castle/network/packet_send.hpp"
#include "castle/writer.hpp"
#include "castleProtocol/protocol/play.hpp"
#include <cstdint>

namespace castle::handlers::play {
  void gameEvent(castle::network::TcpConnection& connection, GameEvent event, float value) {
    PlayPacketWriter packet(protocol::PlayClientbound::GameEvent);

    packet.addByte(static_cast<std::uint8_t>(event));
    packet.addFloat(value);

    network::sendPacket(connection, packet);
  }

  PlayPacketWriter systemMessage(const nbt::Nbt& message, bool type) {

    PlayPacketWriter packet(protocol::PlayClientbound::SystemChatMessage);

    packet.addBytes(nbt::NbtWriter::writer(message, {.namelessRoot = true}));
    packet.addBoolean(type);

    return packet;
  }
} // namespace castle::handlers::play
