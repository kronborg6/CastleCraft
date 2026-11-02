#include "handlers/play.hpp"
#include "client.hpp"
#include "protocol/endian.hpp"
#include "protocol/protocol.hpp"
#include "tcp.hpp"
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

using namespace minecraft::protocol;

namespace minecraft::protocol::handlers {
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
    minecraft::protocol::writeVarInt(10, data);
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
  }
  void handlePlayState(Client& client) {

    TcpConnection& connection = client.connection();

    std::cout << "hello\n";
    std::cout << "hello\n";
    std::cout << "hello\n";
    const int32_t packetLength = minecraft::protocol::readVarInt(connection);

    const int32_t packetId = minecraft::protocol::readVarInt(connection);
    std::cout << "packetLength: " << packetLength << " packetId: " << packetId << "\n";

    switch (packetId) {
    case 0x32:
      std::cout << "hello\n";
    default:
      std::cerr << "Unknown Configuration packet: 0x" << std::hex << packetId << std::dec << '\n';
      break;
    }
  }
} // namespace minecraft::protocol::handlers
