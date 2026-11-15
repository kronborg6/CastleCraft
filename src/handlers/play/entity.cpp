#include "castle/handlers/play/entity.hpp"
#include "castle/client.hpp"
#include "castle/handlers/play/play.hpp"
#include "castle/player/postion.hpp"
#include "castle/server/server.hpp"
#include "castleProtocol/protocol/play.hpp"
#include <cmath>
#include <cstdint>
#include <vector>

namespace castle::handlers::play {
std::uint8_t toAngle(float degrees) {
  float normalized = std::fmod(degrees, 360.0f);

  if (normalized < 0.0f) {
    normalized += 360.0f;
  }

  return static_cast<std::uint8_t>(normalized * 256.0f / 360.0f);
}

void setHeadRotation(Client& client, const player::Position& pos) {

  PlayPacketWriter packet(protocol::PlayClientbound::SetHeadRotation);

  packet.addVarInt(client.getEntityId());
  packet.addByte(toAngle(pos.yaw));

  client.broadcast(packet);
}
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
                bool onGround,
                player::UpdateType type) {

  if (oldPos == newPos) {
    return;
  }

  switch (type) {

  case castle::player::UpdateType::Position: {
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
    break;
  }
  case castle::player::UpdateType::PositionAndRotation: {
    PlayPacketWriter packet(protocol::PlayClientbound::UpdateEntityPositionAndRotation);

    packet.addVarInt(client.getEntityId());

    // Step Count = 0
    packet.addVarInt(onGround ? 1 : 0);

    auto delta = [](double oldValue, double newValue) -> std::int16_t {
      return static_cast<std::int16_t>(std::lround((newValue - oldValue) * 4096.0));
    };

    packet.addInt<std::int16_t>(delta(oldPos.x, newPos.x));
    packet.addInt<std::int16_t>(delta(oldPos.y, newPos.y));
    packet.addInt<std::int16_t>(delta(oldPos.z, newPos.z));

    packet.addByte(toAngle(newPos.yaw));
    packet.addByte(toAngle(newPos.pitch));

    client.broadcast(packet, true);
    setHeadRotation(client, newPos);
    break;
  }
  case castle::player::UpdateType::Rotation: {
    PlayPacketWriter packet(protocol::PlayClientbound::UpdateEntityRotation);

    packet.addVarInt(client.getEntityId());

    // Step Count = 0
    packet.addVarInt(onGround ? 1 : 0);

    packet.addByte(toAngle(newPos.yaw));
    packet.addByte(toAngle(newPos.pitch));

    setHeadRotation(client, newPos);
    client.broadcast(packet, true);
    break;
  }
  };
}
PlayPacketWriter removeEntitys(std::span<const std::int32_t> ids) {
  PlayPacketWriter packet(protocol::PlayClientbound::RemoveEntities);

  packet.addVarIntArray(ids);

  return packet;
}
} // namespace castle::handlers::play
