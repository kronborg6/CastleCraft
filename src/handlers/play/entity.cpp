#include "castle/handlers/play/entity.hpp"
#include "castle/handlers/play/play.hpp"
#include "castleProtocol/package/writePacket.hpp"
#include "castleProtocol/protocol/play.hpp"

namespace castle::handlers::play {
  PlayPacketWriter addPlayerEntity(const Client& client) {
    PlayPacketWriter packet(castle::protocol::PlayClientbound::SpawnEntity);

    auto profile = client.profile;
    auto postion = client.getPlayer().getPostion();

    packet.addVarInt(client.getEntityId());
    packet.addBytes(profile.uuid);
    packet.addVarInt(159);

    packet.addDouble(0);
    packet.addDouble(64);
    packet.addDouble(0);
    packet.addByte(0x00);
    packet.addByte(0);
    packet.addByte(10);
    packet.addByte(0);
    packet.addVarInt(2);

    return packet;
  }
} // namespace castle::handlers::play
