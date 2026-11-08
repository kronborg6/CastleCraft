#include "handlers/play.hpp"
#include "castle/network/packet_receiver.hpp"
#include "castle/network/packet_send.hpp"
#include "castle/network/tcp_connection.hpp"
#include "castleProtocol/package/readPackage.hpp"
#include "castleProtocol/package/writePacket.hpp"
#include "castleProtocol/protocol/play.hpp"
#include "client.hpp"
#include "protocol/endian.hpp"
#include "protocol/packget.hpp"
#include "protocol/protocol.hpp"
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

using namespace minecraft::protocol;

namespace {} // namespace

namespace minecraft::protocol::handlers {
  void start(csatle::network::TcpConnection& connection) {

    castle::protocol::PacketWriter<castle::protocol::PlayClientbound> packet(
        castle::protocol::PlayClientbound::GameEvent);

    packet.addByte(0xd);
    packet.addFloat(0.0f);

    castle::network::sendPacket(connection, packet);
  }

  void setCenterChuck(csatle::network::TcpConnection& connection) {
    std::vector<std::uint8_t> data;
    writeVarInt(0, data);
    writeVarInt(0, data);

    castle::protocol::PacketWriter<castle::protocol::PlayClientbound> packet(
        castle::protocol::PlayClientbound::SetCenterChunk);

    castle::network::sendPacket(connection, packet);
  }

  void startCuck(csatle::network::TcpConnection& connection) {
    std::vector<std::uint8_t> data;

    castle::protocol::PacketWriter<castle::protocol::PlayClientbound> packet(
        castle::protocol::PlayClientbound::ChunkBatchStart);
    castle::network::sendPacket(connection, packet);
  }
  void finishChunkBatch(csatle::network::TcpConnection& connection, std::int32_t chucks) {
    castle::protocol::PacketWriter<castle::protocol::PlayClientbound> packet(
        castle::protocol::PlayClientbound::ChunkBatchFinished);
    packet.addInt<std::int32_t>(chucks);
    castle::network::sendPacket(connection, packet);
  }

  void chuck(csatle::network::TcpConnection& connection, std::int32_t x, std::int32_t y) {

    std::vector<std::uint8_t> sections;

    castle::protocol::PacketWriter<castle::protocol::PlayClientbound> packet(
        castle::protocol::PlayClientbound::ChunkDataAndUpdateLight);

    // Chunk coordinates (Int)
    packet.addVarInt(x);
    packet.addVarInt(y);

    // Heightmaps: 0 entries
    packet.addVarInt(0);

    // Standard Overworld: 384 / 16 = 24 sections
    //

    for (int i = 0; i < 24; ++i) {
      bool randomBlock = (i == 7);
      // Non-air block count (Short)
      packet.addInt<std::int16_t>(randomBlock ? 4096 : 0);

      // Fluid count (Short)
      packet.addInt<std::int16_t>(0);

      // Block states: single-valued palette
      sections.push_back(0x00); // Bits per entry = 0
      minecraft::protocol::writeVarInt(randomBlock ? rand() % 25 : 0, sections); // Air

      // Biomes: single-valued palette
      sections.push_back(0x00);                      // Bits per entry = 0
      minecraft::protocol::writeVarInt(0, sections); // Biome ID 0
    }

    // Section data: VarInt length + raw bytes
    packet.addVarInt(static_cast<std::int32_t>(sections.size()));

    data.insert(data.end(), sections.begin(), sections.end());

    // Block entities: none
    packet.addVarInt(0);

    // Sky light mask: empty
    packet.addVarInt(0);

    // Block light mask: empty
    packet.addVarInt(0);

    // Empty sky light mask: empty
    packet.addVarInt(0);

    // Empty block light mask: empty
    packet.addVarInt(0);

    // Sky light arrays: none
    packet.addVarInt(0);

    // Block light arrays: none
    packet.addVarInt(0);

    castle::network::sendPacket(connection, packet);
  }

  void playerPostion(Client& clinet) {
    std::vector<std::uint8_t> data;

    castle::protocol::PacketWriter<castle::protocol::PlayClientbound> packet(
        castle::protocol::PlayClientbound::SynchronizePlayerPosition);

    std::int32_t teleportId = clinet.getPlayer().newTeleport();

    packet.addVarInt(teleportId);

    packet
        .addInt<std::int64_t>(T value)

            packet.addInt<std::int64_t>(endian::doubleToint64(0));
    packet.addInt<std::int64_t>(endian::doubleToint64(64));
    packet.addInt<std::int64_t>(endian::doubleToint64(0));

    packet.addInt<std::int64_t>(endian::doubleToint64(0.0));
    packet.addInt<std::int64_t>(endian::doubleToint64(0.0));
    packet.addInt<std::int64_t>(endian::doubleToint64(0.0));

    packet.addInt<std::int64_t>(endian::floatToint32(0));
    packet.addInt<std::int64_t>(endian::floatToint32(0));

    packet.addInt<std::int64_t>(0);

    castle::network::sendPacket(clinet.connection(), packet);
  }

  void initialiizePlay(Client& clinet) {
    csatle::network::TcpConnection& connection = clinet.connection();
    std::vector<uint8_t> data;

    // Player Enitity ID
    endian::writeBigEndian<std::int32_t>(clinet.getEntityId(), data);
    // Is Hardcore
    data.push_back(0x00);
    // Dimension Names
    std::vector<std::string> dimensions = {"minecraft:overworld"};
    minecraft::protocol::writeVarInt(static_cast<std::int32_t>(dimensions.size()), data);
    for (const auto& dimension : dimensions) {
      minecraft::protocol::writeString(dimension, data);
    }
    // Max Players
    minecraft::protocol::writeVarInt(12, data);
    // View Distance
    minecraft::protocol::writeVarInt(2, data);
    // Simulation Distance
    minecraft::protocol::writeVarInt(2, data);
    // Reduced Debug Info
    data.push_back(0x00);
    // Enable respawn screen
    data.push_back(0x01);
    // Do limited crafting
    data.push_back(0x00);
    // Dimension Type
    minecraft::protocol::writeVarInt(0, data);
    // Dimension Name
    minecraft::protocol::writeString("minecraft:overworld", data);
    // Hashed seed (not used at the moment just wanna join)
    endian::writeBigEndian<std::int64_t>(std::uint64_t{0}, data);
    // Game mode
    minecraft::protocol::writeVarInt(1, data);
    // Previous Game mode
    minecraft::protocol::writeVarInt(0, data);
    // is Debug (flase)
    data.push_back(0x00);
    // Is Flat (false)
    data.push_back(0x00);
    // Has death location
    data.push_back(0x00);
    // Portal cooldown
    minecraft::protocol::writeVarInt(1000, data);
    // Sea Level
    minecraft::protocol::writeVarInt(20, data);
    // Online mode
    data.push_back(0x01);
    // Enforces Secure Chat
    data.push_back(0x00);

    minecraft::protocol::sendPacket(connection, 0x32, data);

    start(connection);
    playerPostion(clinet);
    setCenterChuck(connection);
    startCuck(connection);

    for (int i = -2; i <= 2; i++) {
      for (int x = -2; x <= 2; x++) {
        chuck(connection, i, x);
      }
    }
    finishChunkBatch(connection, 25);
  }

  // TODO
  // DONE Game Event (0x27) — Send event 13 to tell the client to start waiting for chunks.
  // DONE (not barebone but close) Synchronize Player Position (0x49) — Set the player's spawn
  // position and rotation. DONE (barebone) Set Center Chunk (0x60) — Tell the client which chunk to
  // load around. DONE (barebone) Chunk Data and Update Light (0x2E) — Send terrain and lighting
  // information. Keep Alive (0x2D) — Keep the connection alive.

  void handlePlayState(Client& client) {
    csatle::network::TcpConnection& connection = client.connection();

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
        std::cout << "ping reeust did not match expted: " << client.lastPing << "\ngot: " << value
                  << "\n";
        std::cout << "ping reeust did not match expted: " << client.lastPing << "\ngot: " << value
                  << "\n";
        std::cout << "ping reeust did not match expted: " << client.lastPing << "\ngot: " << value
                  << "\n";
        std::cout << "ping reeust did not match expted: " << client.lastPing << "\ngot: " << value
                  << "\n";
        std::cout << "ping reeust did not match expted: " << client.lastPing << "\ngot: " << value
                  << "\n";
      }
      std::cout << "keep alive match\n";
      break;
    }

    default:
      // std::cerr << "Unknown Configuration packet: 0x" << std::hex << packget.packetId << std::dec
      //           << '\n';
      break;
    }
  }
} // namespace minecraft::protocol::handlers
