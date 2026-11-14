#include "castle/handlers/play/movment.hpp"
#include "castle/handlers/play/play.hpp"

namespace castle::handlers::play {
  void SetPlayerPositionAndRotation(PlayPacketReader& recive, Client& client) {
  }
  void SetPlayerPosition(PlayPacketReader& recive, Client& client);
  void SetPlayerRotation(PlayPacketReader& recive, Client& client);

  void teleportPlayer(Client& client, player::Position postion) {
    PlayPacketWriter packet(castle::protocol::PlayClientbound::SynchronizePlayerPosition);
    std::int32_t teleportId = client.getPlayer().newTeleport();

    packet.addVarInt(teleportId);

    packet.addDouble(postion.x);
    packet.addDouble(postion.y);
    packet.addDouble(postion.z);

    // Velocity is not jet on postion im not use if im gona add it
    packet.addDouble(0);
    packet.addDouble(0);
    packet.addDouble(0);

    packet.addFloat(postion.yaw);
    packet.addFloat(postion.pitch);

    packet.addInt<std::uint32_t>(0);

    castle::network::sendPacket(client.connection(), packet);
  }
} // namespace castle::handlers::play
