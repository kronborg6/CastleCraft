#include "castle/handlers/play/play.hpp"

#include "castle/client.hpp"
#include "castle/handlers/play/chunk.hpp"
#include "castle/network/packet_receiver.hpp"
#include "castle/network/tcp_connection.hpp"
#include "castle/player/player.hpp"
#include "castle/player/postion.hpp"
#include "castle/server/server.hpp"
#include "castleProtocol/package/readPackage.hpp"
#include "castleProtocol/protocol/play.hpp"
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <vector>

namespace castle::handlers::play {
  void handlePlayState(server::Server& server, Client& client) {
    castle::network::TcpConnection& connection = client.connection();

    std::vector<std::uint8_t> data = castle::network::receivePacket(connection);

    castle::protocol::PacketReader<castle::protocol::PlayServerbound> packet(data);

    // Packget packget = createPacket(connection);

    switch (packet.PacketId) {
    case castle::protocol::PlayServerbound::ConfirmTeleportation: {
      std::int32_t teleportId = packet.varInt();

      std::cout << "palyerTeleportId: " << client.getPlayer().teleportId() << "\n"
                << "TeleportId: " << teleportId << std::endl;

      if (!client.getPlayer().checkTeleport(teleportId)) {
        std::cout << "teleport id did not match\n";
      }
      std::cout << "telportId did match\n";

      break;
    }
    case castle::protocol::PlayServerbound::ResourcePackResponse:
      break;
    case castle::protocol::PlayServerbound::KeepAlive: {
      auto value = packet.getInt<std::int64_t>();
      if (value != client.lastPing) {
        // close connection send player left or timeout to other players
      }
      break;
    }
    case castle::protocol::PlayServerbound::SetPlayerPosition: {
      castle::player::Position p = {
          .x = packet.getDouble(),
          .y = packet.getDouble(),
          .z = packet.getDouble(),
      };
      if (client.getPlayer().updatePostion(p, player::UpdateType::Position)) {
        play::updatePlayerLoadeChunks(client);
      }
      break;
    }

    case castle::protocol::PlayServerbound::SetPlayerPositionAndRotation: {
      castle::player::Position p = {
          .x = packet.getDouble(),
          .y = packet.getDouble(),
          .z = packet.getDouble(),
          .yaw = packet.getFloat(),
          .pitch = packet.getFloat(),
          .flags = packet.byte(),
      };

      if (client.getPlayer().updatePostion(p, player::UpdateType::PositionAndRotation)) {
        play::updatePlayerLoadeChunks(client);
      }

      break;
    }
    case castle::protocol::PlayServerbound::SetPlayerRotation: {
      castle::player::Position p = {
          .yaw = packet.getFloat(),
          .pitch = packet.getFloat(),
          .flags = packet.byte(),
      };

      auto newChunk = client.getPlayer().updatePostion(p, player::UpdateType::Rotation);
      // std::cout << "yaw: " << p.yaw << "\npich: " << p.pitch << "\nbyte: " << +p.flags << "\n";

      std::cout << "Player rotation\n";
      break;
    }
    default:
      // std::cerr << "Unknown Configuration packet: 0x" << std::hex
      //           << static_cast<std::int32_t>(packet.PacketId) << std::dec << '\n';
      break;
    }
  }
} // namespace castle::handlers::play
