#include "castle/handlers/play/entity.hpp"
#include "castle/client.hpp"
#include "castle/handlers/play/play.hpp"
#include "castle/player/postion.hpp"
#include "castle/server/server.hpp"
#include "castleProtocol/protocol/play.hpp"
#include <cmath>
#include <cstdint>

namespace castle::handlers::play {
  PlayPacketWriter addPlayerEntity(const Client& client) {
    PlayPacketWriter packet(castle::protocol::PlayClientbound::SpawnEntity);

    auto profile = client.profile;
    auto postion = client.getPlayer().getPostion();

    packet.addVarInt(client.getEntityId());
    packet.addBytes(profile.uuid);
    packet.addVarInt(159);

    packet.addDouble(postion.x);
    packet.addDouble(postion.y);
    packet.addDouble(postion.z);

    // packet.addDouble(0);
    // packet.addDouble(64);
    // packet.addDouble(0);
    packet.addByte(0x00);
    packet.addByte(0);
    packet.addByte(10);
    packet.addByte(0);
    packet.addVarInt(2);

    return packet;
  }
  void moveEntity(Client& client,
                  const player::Position& oldPos,
                  const player::Position& newPos,
                  bool onGround) {

    if (oldPos == newPos) {
      return;
    }

    PlayPacketWriter packet(protocol::PlayClientbound::UpdateEntityPosition);

    packet.addVarInt(client.getEntityId());

    // Step Count = 0
    packet.addVarInt(onGround ? 1 : 0);

    auto delta = [](double oldValue, double newValue) -> std::int16_t {
      return static_cast<std::int16_t>(std::lround((newValue - oldValue) * 4096.0));
    };

    packet.addInt<std::int16_t>(delta(oldPos.x, newPos.x));
    packet.addInt<std::int16_t>(delta(oldPos.y, newPos.y));
    packet.addInt<std::int16_t>(delta(oldPos.z, newPos.z));

    client.broadcast(packet, true);
  }
} // namespace castle::handlers::play
