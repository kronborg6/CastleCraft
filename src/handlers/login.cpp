#include "client.hpp"
#include "protocol/protocol.hpp"
#include "tcp.hpp"
#include <algorithm>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <random>
#include <string>
#include <uuid/uuid.h>

namespace {
  std::string uuidToString(const std::array<uint8_t, 16>& uuid) {
    std::ostringstream stream;

    for (std::size_t i = 0; i < uuid.size(); ++i) {
      if (i == 4 || i == 6 || i == 8 || i == 10) {
        stream << '-';
      }

      stream << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(uuid[i]);
    }

    return stream.str();
  }
  std::array<uint8_t, 16> generateUuid() {
    std::array<uint8_t, 16> uuid{};

    std::random_device randomDevice;
    std::mt19937 generator(randomDevice());
    std::uniform_int_distribution<int> distribution(0, 255);

    for (auto& byte : uuid) {
      byte = static_cast<uint8_t>(distribution(generator));
    }

    // Version 4 UUID
    uuid[6] = static_cast<uint8_t>((uuid[6] & 0x0F) | 0x40);

    // RFC 4122 variant
    uuid[8] = static_cast<uint8_t>((uuid[8] & 0x3F) | 0x80);

    return uuid;
  }

  void handleLoginRequst(Client& client) {

    TcpConnection& connection = client.connection();

    std::string username = minecraft::protocol::readString(connection);
    std::array<uint8_t, 16> uuid{};

    connection.readAll(uuid.data(), uuid.size());

    std::cout << "uuid: " << uuidToString(uuid) << "\nusername: " << username << "\n";

    client.profile = {.uuid = uuid, .username = username, .properties = {}};

    std::vector<uint8_t> data;

    data.insert(data.end(), uuid.begin(), uuid.end());

    minecraft::protocol::writeString(username, data);
    minecraft::protocol::writeVarInt(0, data);

    std::array<uint8_t, 16> sessionId = generateUuid();

    data.insert(data.end(), sessionId.begin(), sessionId.end());

    minecraft::protocol::sendPacket(connection, 0x02, data);
    std::cout << "send login susch\n";
  }

} // namespace
namespace minecraft::protocol::handlers {
  void handleLogin(Client& client) {
    TcpConnection& connection = client.connection();
    int32_t length = minecraft::protocol::readVarInt(connection);
    int32_t packetId = minecraft::protocol::readVarInt(connection);

    switch (packetId) {
    case 0x00:
      handleLoginRequst(client);
      break;
    case 0x03:
      client.updateState(minecraft::protocol::State::Configuration);
      break;
    default:
      std::cerr << "Unknown Configuration packet: 0x" << std::hex << packetId << std::dec << '\n';
      break;
    }

    std::cout << "length: " << length << "\nacketId: " << packetId << "\n";
  }
} // namespace minecraft::protocol::handlers
