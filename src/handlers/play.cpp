#include "handlers/play.hpp"
#include "client.hpp"
#include "protocol/endian.hpp"
#include "protocol/packget.hpp"
#include "protocol/protocol.hpp"
#include "tcp.hpp"
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

using namespace minecraft::protocol;

namespace {} // namespace

namespace minecraft::protocol::handlers {
  void start(TcpConnection& connection) {
    std::vector<std::uint8_t> data;

    data.push_back(0xd);
    std::int32_t value = minecraft::protocol::endian::floatToint32(0.0f);

    minecraft::protocol::endian::writeBigEndian<std::int32_t>(value, data);

    minecraft::protocol::sendPacket(connection, 0x27, data);
  }

  void setCenterChuck(TcpConnection& connection) {
    std::vector<std::uint8_t> data;
    writeVarInt(0, data);
    writeVarInt(0, data);

    sendPacket(connection, 0x60, data);
  }

  void startCuck(TcpConnection& connection) {
    std::vector<std::uint8_t> data;
    data.push_back(0x00);
    sendPacket(connection, 0x0C, data);
  }
  void finishChunkBatch(TcpConnection& connection, std::int32_t chucks) {
    std::vector<std::uint8_t> data;
    writeVarInt(chucks, data);
    sendPacket(connection, 0x0B, data);
  }

  void chuck(TcpConnection& connection, std::int32_t x, std::int32_t y) {
    std::vector<std::uint8_t> data;
    std::vector<std::uint8_t> sections;

    // Chunk coordinates (Int)
    endian::writeBigEndian<std::int32_t>(x, data);
    endian::writeBigEndian<std::int32_t>(y, data);

    // Heightmaps: 0 entries
    minecraft::protocol::writeVarInt(0, data);

    // Standard Overworld: 384 / 16 = 24 sections
    //

    for (int i = 0; i < 24; ++i) {
      bool isStone = (i == 7);
      // Non-air block count (Short)
      endian::writeBigEndian<std::int16_t>(isStone ? 4096 : 0, sections);

      // Fluid count (Short)
      endian::writeBigEndian<std::int16_t>(0, sections);

      // Block states: single-valued palette
      sections.push_back(0x00);                                    // Bits per entry = 0
      minecraft::protocol::writeVarInt(isStone ? 1 : 0, sections); // Air

      // Biomes: single-valued palette
      sections.push_back(0x00);                      // Bits per entry = 0
      minecraft::protocol::writeVarInt(0, sections); // Biome ID 0
    }

    // Section data: VarInt length + raw bytes
    minecraft::protocol::writeVarInt(static_cast<std::int32_t>(sections.size()), data);

    data.insert(data.end(), sections.begin(), sections.end());

    // Block entities: none
    minecraft::protocol::writeVarInt(0, data);

    // Sky light mask: empty
    minecraft::protocol::writeVarInt(0, data);

    // Block light mask: empty
    minecraft::protocol::writeVarInt(0, data);

    // Empty sky light mask: empty
    minecraft::protocol::writeVarInt(0, data);

    // Empty block light mask: empty
    minecraft::protocol::writeVarInt(0, data);

    // Sky light arrays: none
    minecraft::protocol::writeVarInt(0, data);

    // Block light arrays: none
    minecraft::protocol::writeVarInt(0, data);

    minecraft::protocol::sendPacket(connection, 0x2E, data);
  }

  void playerPostion(Client& clinet) {
    std::vector<std::uint8_t> data;

    std::int32_t teleportId = clinet.getPlayer().newTeleport();

    minecraft::protocol::writeVarInt(teleportId, data);

    endian::writeBigEndian<std::int64_t>(endian::doubleToint64(0), data);
    endian::writeBigEndian<std::int64_t>(endian::doubleToint64(64), data);
    endian::writeBigEndian<std::int64_t>(endian::doubleToint64(0), data);

    endian::writeBigEndian<std::int64_t>(endian::doubleToint64(0.0), data);
    endian::writeBigEndian<std::int64_t>(endian::doubleToint64(0.0), data);
    endian::writeBigEndian<std::int64_t>(endian::doubleToint64(0.0), data);

    endian::writeBigEndian<std::int32_t>(endian::floatToint32(0), data);
    endian::writeBigEndian<std::int32_t>(endian::floatToint32(0), data);

    endian::writeBigEndian<std::uint32_t>(0, data);

    minecraft::protocol::sendPacket(clinet.connection(), 0x49, data);
  }

  void initialiizePlay(Client& clinet) {
    TcpConnection& connection = clinet.connection();
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

    for (int i = 0; i < 16; i++) {
      for (int x = 0; x < 16; x++) {
        chuck(connection, i, x);
      }
    }
    finishChunkBatch(connection, 16 * 16);
  }

  // TODO
  // DONE Game Event (0x27) — Send event 13 to tell the client to start waiting for chunks.
  // Synchronize Player Position (0x49) — Set the player's spawn position and rotation.
  // Set Center Chunk (0x60) — Tell the client which chunk to load around.
  // Chunk Data and Update Light (0x2E) — Send terrain and lighting information.
  // Keep Alive (0x2D) — Keep the connection alive.

  void handlePlayState(Client& client) {

    TcpConnection& connection = client.connection();

    Packget packget = createPacket(connection);
    std::cout << "packetLength: " << packget.data.size() << " packetId: " << packget.packetId
              << "\n";

    switch (packget.packetId) {
    case 0x00: {
      std::size_t offset = 0;
      std::int32_t teleportId = minecraft::protocol::readVarInt(packget.data, offset);

      std::cout << "palyerTeleportId: " << client.getPlayer().teleportId() << "\n"
                << "TeleportId: " << teleportId << std::endl;

      std::cout << "palyerTeleportId: " << client.getPlayer().teleportId() << "\n"
                << "TeleportId: " << teleportId << std::endl;
      std::cout << "palyerTeleportId: " << client.getPlayer().teleportId() << "\n"
                << "TeleportId: " << teleportId << std::endl;
      std::cout << "palyerTeleportId: " << client.getPlayer().teleportId() << "\n"
                << "TeleportId: " << teleportId << std::endl;
      std::cout << "palyerTeleportId: " << client.getPlayer().teleportId() << "\n"
                << "TeleportId: " << teleportId << std::endl;

      if (teleportId != client.getPlayer().teleportId()) {
        std::cout << "teleport id did not match\n";
      }
      std::cout << "telportId did match: " << teleportId << "\n";

      break;
    }
    case 0x32:
    default:
      std::cerr << "Unknown Configuration packet: 0x" << std::hex << packget.packetId << std::dec
                << '\n';
      break;
    }
  }
} // namespace minecraft::protocol::handlers
