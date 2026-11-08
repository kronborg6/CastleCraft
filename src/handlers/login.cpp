#include "castleProtocol/protocol/login.hpp"
#include "castle/client.hpp"
#include "castle/network/packet_receiver.hpp"
#include "castle/network/packet_send.hpp"
#include "castle/network/tcp_connection.hpp"
#include "castleProtocol/package/readPackage.hpp"
#include <algorithm>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <random>
#include <span>
#include <string>
#include <uuid/uuid.h>
#include <vector>

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

  void
  handleLoginRequst(Client& client,
                    castle::protocol::PacketReader<castle::protocol::LoginServerbound>& packet) {
    auto& connection = client.connection();

    std::string username = packet.string();

    std::array<std::uint8_t, 16> uuid{};
    auto uuidBytes = packet.bytes(16);

    std::ranges::copy(uuidBytes, uuid.begin());

    std::cout << "uuid: " << uuidToString(uuid) << "\nusername: " << username << '\n';

    client.profile = {.uuid = uuid, .username = username, .properties = {}};

    castle::protocol::PacketWriter<castle::protocol::LoginClientbound> writer(
        castle::protocol::LoginClientbound::LoginSuccess);

    // Game Profile
    writer.addBytes(uuid);
    writer.addString(username);
    writer.addVarInt(0); // No properties

    // Session ID
    writer.addBytes(generateUuid());

    castle::network::sendPacket(connection, writer);

    std::cout << "Sent Login Success\n";
  }

} // namespace
namespace castle::handlers {
  void handleLogin(Client& client) {
    csatle::network::TcpConnection& connection = client.connection();

    std::vector<std::uint8_t> data = castle::network::receivePacket(connection);

    castle::protocol::PacketReader<castle::protocol::LoginServerbound> packet(data);

    switch (packet.PacketId) {
    case castle::protocol::LoginServerbound::LoginStart:
      handleLoginRequst(client, packet);
      break;
    case castle::protocol::LoginServerbound::LoginAcknowledged:
      client.updateState(castle::protocol::State::Configuration);
      break;
    default:
      std::cerr << "Unknown Configuration packet: 0x" << std::hex
                << static_cast<std::int32_t>(packet.PacketId) << std::dec << '\n';
      break;
    }
  }
} // namespace castle::handlers
