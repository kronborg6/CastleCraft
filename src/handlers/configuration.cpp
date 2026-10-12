#include "handlers/configuration.hpp"
#include "client.hpp"
#include "protocol/protocol.hpp"
#include "tcp.hpp"
#include <cstdint>
#include <iostream>
#include <vector>

namespace {
  int32_t readVarInt2(std::vector<std::uint8_t>& data) {
    int32_t value = 0;
    int position = 0;

    while (true) {
      const uint8_t byte = data.at(1);

      value |= (byte & 0x7F) << position;

      if ((byte & 0x80) == 0) {
        break;
      }

      position += 7;

      if (position >= 32) {
        throw std::runtime_error("VarInt too large");
      }
    }

    return value;
  }
  struct Packget {
      int32_t packetId;
      std::vector<std::uint8_t> data;
  };
  Packget createPacket(TcpConnection& connection) {
    const int32_t packetLength = minecraft::protocol::readVarInt(connection);

    const int32_t packetId = minecraft::protocol::readVarInt(connection);

    if (packetLength < 1) {
      throw std::runtime_error("Invalid packet length");
    }

    const std::size_t payloadLength = static_cast<std::size_t>(packetLength - 1);

    std::vector<std::uint8_t> data(payloadLength);

    connection.readAll(data.data(), data.size());

    return Packget{packetId, std::move(data)};
  }
  void sendEnty(TcpConnection& connection) {
    std::vector<uint8_t> data;

    minecraft::protocol::writeString("minecraft:dimension_type", data);

    // zero entries
    minecraft::protocol::writeVarInt(0, data);

    minecraft::protocol::sendPacket(connection, 0x07, data);
  }
} // namespace

namespace minecraft::protocol::handlers {
  void handleConfiguration(Client& client) {
    TcpConnection& connection = client.connection();

    Packget packet = createPacket(connection);

    std::vector<std::uint8_t> temp;
    std::cout << "we now at: " << packet.packetId << "\n";

    if (client.sent == false) {
      sendEnty(connection);
      minecraft::protocol::sendPacket(connection, 0x03, {});
      client.sent = true;
    }

    switch (packet.packetId) {
    case 0x00:
      // minecraft::protocol::readVarInt(connection);
      std::cout << "0\n";
      break;
    case 0x01:
      // minecraft::protocol::readVarInt(connection);
      std::cout << "1\n";
      break;
    case 0x02:
      // minecraft::protocol::readVarInt(connection);
      std::cout << "2\n";
      break;
    case 0x03:
      // minecraft::protocol::sendPacket(connection, 0x03, {});
      client.updateState(minecraft::protocol::State::Play);
      std::cout << "3\n";
      break;
    case 0x04:
      // minecraft::protocol::readVarInt(connection);
      std::cout << "4\n";
      break;
    case 0x05:
      // int gg = readVarInt2(packet.data);

      // minecraft::protocol::writeVarInt(0, temp);
      // minecraft::protocol::sendPacket(connection, 0x05, temp);
      std::cout << "5\n";
      break;
    case 0x06:
      // minecraft::protocol::readVarInt(connection);
      std::cout << "6\n";
      break;
    case 0x07:
      // minecraft::protocol::readVarInt(connection);
      std::cout << "7\n";
      break;
    case 0x08:
      // minecraft::protocol::readVarInt(connection);
      std::cout << "8\n";
      break;
    case 0x09:
      // minecraft::protocol::readVarInt(connection);
      std::cout << "9\n";
      break;
    default:
      std::cerr << "Unknown Configuration packet: 0x" << std::hex << packet.packetId << std::dec
                << '\n';
      break;
      break;
    }
  }
} // namespace minecraft::protocol::handlers
