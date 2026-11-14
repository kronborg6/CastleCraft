#include "castle/handlers/play.hpp"
#include "castle/client.hpp"
#include "castle/handlers/play/chunk.hpp"
#include "castle/network/packet_receiver.hpp"
#include "castle/network/packet_send.hpp"
#include "castle/network/tcp_connection.hpp"
#include "castle/player/player.hpp"
#include "castle/player/postion.hpp"
#include "castle/server/server.hpp"
#include "castle/world/chunk.hpp"
#include "castle/world/chunkPostion.hpp"
#include "castle/world/woldGreanter.hpp"
#include "castleProtocol/endian.hpp"
#include "castleProtocol/package/readPackage.hpp"
#include "castleProtocol/package/writePacket.hpp"
#include "castleProtocol/protocol/play.hpp"
#include "castleProtocol/protocol/varint.hpp"
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace castle::handlers {

  // void start(castle::network::TcpConnection& connection) {
  //
  //   castle::protocol::PacketWriter<castle::protocol::PlayClientbound> packet(
  //       castle::protocol::PlayClientbound::GameEvent);
  //
  //   packet.addByte(0xd);
  //   packet.addFloat(0.0f);
  //
  //   castle::network::sendPacket(connection, packet);
  // }
  //
  // void setCenterChuck(castle::network::TcpConnection& connection, std::int32_t x, std::int32_t z)
  // {
  //
  //   castle::protocol::PacketWriter<castle::protocol::PlayClientbound> packet(
  //       castle::protocol::PlayClientbound::SetCenterChunk);
  //
  //   packet.addVarInt(x);
  //   packet.addVarInt(z);
  //
  //   castle::network::sendPacket(connection, packet);
  // }
  //
  // void startCuck(castle::network::TcpConnection& connection) {
  //   std::vector<std::uint8_t> data;
  //
  //   castle::protocol::PacketWriter<castle::protocol::PlayClientbound> packet(
  //       castle::protocol::PlayClientbound::ChunkBatchStart);
  //   castle::network::sendPacket(connection, packet);
  // }
  // void finishChunkBatch(castle::network::TcpConnection& connection, std::int32_t chucks) {
  //   castle::protocol::PacketWriter<castle::protocol::PlayClientbound> packet(
  //       castle::protocol::PlayClientbound::ChunkBatchFinished);
  //   packet.addVarInt(chucks);
  //   castle::network::sendPacket(connection, packet);
  // }
  // void sendStoneChunk(castle::network::TcpConnection& connection, world::ChunkPosition position)
  // {
  //   world::Chunk chunk = world::generateStoneChunk(position);
  //
  //   auto packet = world::encodeChunk(chunk);
  //
  //   castle::network::sendPacket(connection, packet);
  // }
  //
  // void chuck(castle::network::TcpConnection& connection, std::int32_t x, std::int32_t z) {
  //
  //   std::vector<std::uint8_t> sections;
  //
  //   castle::protocol::PacketWriter<castle::protocol::PlayClientbound> packet(
  //       castle::protocol::PlayClientbound::ChunkDataAndUpdateLight);
  //
  //   // Chunk coordinates (fixed-width Int, not VarInt)
  //   packet.addInt<std::int32_t>(x);
  //   packet.addInt<std::int32_t>(z);
  //
  //   // One WORLD_SURFACE heightmap. Stone fills Y=48 through Y=63, so the first
  //   // free block is Y=64. Heightmap values are relative to min_y=-64.
  //   std::vector<std::uint64_t> heightmap(37, 0);
  //   constexpr std::uint64_t height = 128;
  //
  //   for (std::size_t column = 0; column < 256; ++column) {
  //     const std::size_t longIndex = column / 7;
  //     const int bitOffset = (column % 7) * 9;
  //
  //     heightmap[longIndex] |= height << bitOffset;
  //   }
  //
  //   packet.addVarInt(1);  // Heightmap count
  //   packet.addVarInt(1);  // WORLD_SURFACE
  //   packet.addVarInt(37); // Packed long count
  //
  //   for (const std::uint64_t value : heightmap) {
  //     packet.addInt<std::int64_t>(static_cast<std::int64_t>(value));
  //   }
  //
  //   // Standard Overworld: 384 / 16 = 24 sections
  //   //
  //
  //   for (int i = 0; i < 24; ++i) {
  //     bool randomBlock = (i == 7);
  //     // Non-air block count (Short)
  //     castle::endian::writeBigEndian<std::int16_t>(randomBlock ? 4096 : 0, sections);
  //
  //     // Fluid count (Short)
  //     castle::endian::writeBigEndian<std::int16_t>(0, sections);
  //
  //     // Block states: single-valued palette
  //     sections.push_back(0x00); // Bits per entry = 0
  //     castle::protocol::writeVarInt(randomBlock ? rand() % 25 + 1 : 0,
  //                                   sections); // Air
  //
  //     // Biomes: single-valued palette
  //     sections.push_back(0x00);                   // Bits per entry = 0
  //     castle::protocol::writeVarInt(0, sections); // Biome ID 0
  //   }
  //
  //   // Section data: VarInt length + raw bytes
  //   packet.addVarInt(static_cast<std::int32_t>(sections.size()));
  //
  //   packet.addBytes(sections);
  //
  //   // Block entities: none
  //   packet.addVarInt(0);
  //
  //   std::vector<std::uint8_t> skyLight(2048, 0xFF);
  //
  //   // Sky light mask: all 26 light sections enabled.
  //   packet.addVarInt(4);
  //   packet.addByte(0xFF);
  //   packet.addByte(0xFF);
  //   packet.addByte(0xFF);
  //   packet.addByte(0x03);
  //
  //   // Block light mask: empty.
  //   packet.addVarInt(0);
  //
  //   // Empty sky light mask: empty.
  //   packet.addVarInt(0);
  //
  //   // Empty block light mask: empty.
  //   packet.addVarInt(0);
  //
  //   // Full-bright sky light arrays for all 26 light sections.
  //   packet.addVarInt(26);
  //   for (int i = 0; i < 26; ++i) {
  //     packet.addVarInt(2048);
  //     packet.addBytes(skyLight);
  //   }
  //
  //   // Block light arrays: none.
  //   packet.addVarInt(0);
  //
  //   castle::network::sendPacket(connection, packet);
  // }
  //
  // void playerPostion(Client& clinet) {
  //
  //   castle::protocol::PacketWriter<castle::protocol::PlayClientbound> packet(
  //       castle::protocol::PlayClientbound::SynchronizePlayerPosition);
  //
  //   std::int32_t teleportId = clinet.getPlayer().newTeleport();
  //
  //   packet.addVarInt(teleportId);
  //
  //   packet.addDouble(8);
  //   packet.addDouble(64);
  //   packet.addDouble(8);
  //
  //   packet.addDouble(0);
  //   packet.addDouble(0);
  //   packet.addDouble(0);
  //
  //   packet.addFloat(0.0f);
  //   packet.addFloat(0.0f);
  //
  //   packet.addInt<std::uint32_t>(0);
  //
  //   castle::network::sendPacket(clinet.connection(), packet);
  // }
  //
  // void unloadChunk(castle::network::TcpConnection& connection, world::ChunkPosition postion) {
  //   protocol::PacketWriter<castle::protocol::PlayClientbound> packet(
  //       protocol::PlayClientbound::UnloadChunk);
  //
  //   packet.addInt<std::int32_t>(postion.z);
  //   packet.addInt<std::int32_t>(postion.x);
  //
  //   network::sendPacket(connection, packet);
  // }
  //
  // void updateChunks(Client& client) {
  //   auto& connection = client.connection();
  //
  //   auto& player = client.getPlayer();
  //   const auto center = player.getChunkPostion();
  //
  //   for (auto it = client.loadedChunks.begin(); it != client.loadedChunks.end();) {
  //
  //     const auto& pos = *it;
  //
  //     if (std::abs(pos.x - center.x) > player::VIEW || std::abs(pos.z - center.z) > player::VIEW)
  //     {
  //
  //       unloadChunk(connection, pos);
  //       it = client.loadedChunks.erase(it);
  //     } else {
  //       ++it;
  //     }
  //   }
  //
  //   setCenterChuck(connection, center.x, center.z);
  //
  //   std::vector<world::ChunkPosition> chunksToSend;
  //
  //   for (int x = -player::VIEW; x <= player::VIEW; ++x) {
  //     for (int z = -player::VIEW; z <= player::VIEW; ++z) {
  //
  //       world::ChunkPosition position{center.x + x, center.z + z};
  //
  //       if (!client.loadedChunks.contains(position)) {
  //         chunksToSend.push_back(position);
  //       }
  //     }
  //   }
  //
  //   if (chunksToSend.empty()) {
  //     return;
  //   }
  //
  //   startCuck(connection);
  //
  //   for (const auto& position : chunksToSend) {
  //     sendStoneChunk(connection, position);
  //     client.loadedChunks.insert(position);
  //   }
  //
  //   finishChunkBatch(connection, static_cast<std::int32_t>(chunksToSend.size()));
  // }
  //
  // void initialiizePlay(Client& clinet) {
  //   castle::network::TcpConnection& connection = clinet.connection();
  //
  //   castle::protocol::PacketWriter<castle::protocol::PlayClientbound> packet(
  //       castle::protocol::PlayClientbound::Login);
  //
  //   // Player Enitity ID
  //   packet.addInt<std::int32_t>(clinet.getEntityId());
  //   // Is Hardcore
  //   packet.addByte(0x00);
  //   // Dimension Names
  //   std::vector<std::string> dimensions = {"minecraft:overworld"};
  //   packet.addVarInt(static_cast<std::int32_t>(dimensions.size()));
  //   for (const auto& dimension : dimensions) {
  //     packet.addString(dimension);
  //   }
  //   // Max Players
  //   packet.addVarInt(12);
  //   // View Distance
  //   packet.addVarInt(player::VIEW);
  //   // Simulation Distance
  //   packet.addVarInt(player::VIEW);
  //   // Reduced Debug Info
  //   packet.addByte(0x00);
  //   // Enable respawn screen
  //   packet.addByte(0x01);
  //   // Do limited crafting
  //   packet.addByte(0x00);
  //   // Dimension Type
  //   packet.addVarInt(0);
  //   // Dimension Name
  //   packet.addString("minecraft:overworld");
  //   // Hashed seed (not used at the moment just wanna join)
  //   packet.addInt<std::int64_t>(std::uint64_t{0});
  //   // Game mode
  //   packet.addVarInt(1);
  //   // Previous Game mode
  //   packet.addVarInt(0);
  //   // is Debug (flase)
  //   packet.addByte(0x00);
  //   // Is Flat (false)
  //   packet.addByte(0x00);
  //   // Has death location
  //   packet.addByte(0x00);
  //   // Portal cooldown
  //   packet.addVarInt(1000);
  //   // Sea Level
  //   packet.addVarInt(20);
  //   // Online mode
  //   packet.addByte(0x01);
  //   // Enforces Secure Chat
  //   packet.addByte(0x00);
  //
  //   castle::network::sendPacket(connection, packet);
  //
  //   start(connection);
  //   playerPostion(clinet);
  //   setCenterChuck(connection, 0, 0);
  //   startCuck(connection);
  //
  //   for (int i = -2; i <= 2; i++) {
  //     for (int x = -2; x <= 2; x++) {
  //       sendStoneChunk(connection, {i, x});
  //       // chuck(connection, i, x);
  //     }
  //   }
  //   finishChunkBatch(connection, 21);
  //
  //   // auto clients = clinet.get
  //
  //   playerInfoUpdate(clinet, player::PlayerAction::AddPlayer);
  // }

  // TODO
  // update chucks when player move at the moment we get som invisble blcoks becus the server never
  // send new blocks/chunks
  // Center Chunk Data and Update Light (0x2E) — Send terrain and lighting information.

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
      // std::cout << "Player position:\n"
      //           << "x: " << p.x << "\n"
      //           << "feetY: " << p.y << "\n"
      //           << "z: " << p.z << "\n"Enum E
      //           << "flags: " << static_cast<int>(p.flags) << "\n";
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

      // std::cout << "Player position and rotation:\n"
      //           << "x: " << p.x << "\n"
      //           << "feetY: " << p.y << "\n"
      //           << "z: " << p.z << "\n"
      //           << "yaw: " << p.yaw << "\n"
      //           << "pitch: " << p.pitch << "\n"
      //           << "flags: " << static_cast<int>(p.flags) << "\n";
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
} // namespace castle::handlers
