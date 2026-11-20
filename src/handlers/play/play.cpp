#include "castle/handlers/play/play.hpp"

#include "castle/client.hpp"
#include "castle/entries/entries.hpp"
#include "castle/handlers/play/chunk.hpp"
#include "castle/handlers/play/entity.hpp"
#include "castle/handlers/play/inventory.hpp"
#include "castle/helper/print.hpp"
#include "castle/item/item.hpp"
#include "castle/network/packet_receiver.hpp"
#include "castle/network/tcp_connection.hpp"
#include "castle/player/player.hpp"
#include "castle/player/postion.hpp"
#include "castle/server/server.hpp"
#include "castle/world/chunkPostion.hpp"
#include "castleProtocol/endian.hpp"
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
      auto& player = client.getPlayer();
      auto& oldPostion = player.getPostion();
      castle::player::Position p = {
          .x = packet.getDouble(),
          .y = packet.getDouble(),
          .z = packet.getDouble(),
      };
      moveEntity(client, oldPostion, p, true, player::UpdateType::Position);
      if (player.updatePostion(p, player::UpdateType::Position)) {
        play::updatePlayerLoadeChunks(server, client);
      }
      // helper::print::printMovemet(client);
      break;
    }

    case castle::protocol::PlayServerbound::SetPlayerPositionAndRotation: {
      auto& player = client.getPlayer();
      auto& oldPostion = player.getPostion();
      castle::player::Position p = {
          .x = packet.getDouble(),
          .y = packet.getDouble(),
          .z = packet.getDouble(),
          .yaw = packet.getFloat(),
          .pitch = packet.getFloat(),
          .flags = packet.byte(),
      };
      // helper::print::printMovemet(client);

      moveEntity(client, oldPostion, p, true, player::UpdateType::PositionAndRotation);
      if (player.updatePostion(p, player::UpdateType::PositionAndRotation)) {
        play::updatePlayerLoadeChunks(server, client);
      }

      break;
    }
    case castle::protocol::PlayServerbound::SetPlayerRotation: {
      auto& player = client.getPlayer();
      auto& oldPostion = player.getPostion();
      castle::player::Position p = {
          .yaw = packet.getFloat(),
          .pitch = packet.getFloat(),
          .flags = packet.byte(),
      };

      auto newChunk = player.updatePostion(p, player::UpdateType::Rotation);

      moveEntity(client, oldPostion, p, true, player::UpdateType::Rotation);
      // std::cout << "Player rotation\n";
      break;
    }
    case castle::protocol::PlayServerbound::UseItemOn: {
      auto hand = packet.varInt();
      auto postion = packet.getPosition();
      auto face = packet.varInt();
      auto cursorX = packet.getFloat();
      auto cursorY = packet.getFloat();
      auto cursorZ = packet.getFloat();
      auto insideBlock = packet.byte();
      auto worldBorderHit = packet.byte();
      auto sequence = packet.varInt();

      std::cout << "Hand: " << hand << '\n'
                << "Position: " << postion.x << ", " << postion.y << ", " << postion.z << '\n'
                << "Face: " << face << '\n'
                << "Cursor X: " << cursorX << '\n'
                << "Cursor Y: " << cursorY << '\n'
                << "Cursor Z: " << cursorZ << '\n'
                << "Inside Block: " << static_cast<bool>(insideBlock) << '\n'
                << "World Border Hit: " << static_cast<bool>(worldBorderHit) << '\n'
                << "Sequence: " << sequence << '\n';

      auto inv = client.getPlayer().getInventory();
      auto itemUsed = inv.getHeldItem();

      if (itemUsed != nullptr) {
        server.setBlock(postion, 4);
        std::cout << "ItemID: " << itemUsed->itemId << std::endl;
      }
      break;
    }
    case castle::protocol::PlayServerbound::ClientTickEnd: {
      break;
    }
    case castle::protocol::PlayServerbound::PickItemFromBlock: {
      auto [x, y, z] = packet.getPosition();
      auto includeData = packet.byte();
      std::cout << "Position: " << x << ", " << y << ", " << z << '\n'
                << "IncludeData: " << static_cast<bool>(includeData) << '\n';

      break;
    }
    case castle::protocol::PlayServerbound::SetCreativeModeSlot: {
      auto slotIndex = packet.getInt<std::int16_t>();
      auto slot = packet.getSlot();

      item::Item item = {static_cast<std::uint32_t>(slot.itemId),
                         static_cast<std::uint8_t>(slot.itemCount)};

      updateInventory(client, static_cast<std::uint16_t>(slotIndex), item);

      std::cout << "Slot index: " << slotIndex << '\n';
      std::cout << slot << '\n';

      break;
      break;
    }
    case castle::protocol::PlayServerbound::SetHeldItem: {
      auto hotBarPostion = packet.getShort();

      client.getPlayer().getInventory().updateSelectedHotbarSlot(
          static_cast<std::uint8_t>(hotBarPostion));

      std::cout << "hotbar number: " << client.getPlayer().getInventory().hotbarNumber()
                << std::endl;
      break;
    }
    default:
      std::cerr << "Unknown play packet: 0x" << std::hex
                << static_cast<std::int32_t>(packet.PacketId) << std::dec << '\n';
      break;
    }
  }
} // namespace castle::handlers::play
