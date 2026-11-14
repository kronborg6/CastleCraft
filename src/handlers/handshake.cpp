#include "castle/handlers/handshake.hpp"
#include "castle/network/packet_receiver.hpp"
#include "castle/network/tcp_connection.hpp"
#include "castleProtocol/package/readPackage.hpp"
#include "castleProtocol/protocol/handsake.hpp"
#include <cstdint>
#include <iostream>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

namespace castle::handlers {
  void handleHandshake(Client& client) {
    castle::network::TcpConnection& connection = client.connection();

    std::vector<std::uint8_t> data = castle::network::receivePacket(connection);

    castle::protocol::PacketReader<castle::protocol::HandshakeServerbound> packet(data);

    if (static_cast<int32_t>(packet.PacketId) != 0x00) {
      return;
    }

    int32_t protocolVersion = packet.varInt();
    std::string address = packet.string();

    auto portBytes = packet.bytes(2);

    const uint16_t port = static_cast<uint16_t>((static_cast<uint16_t>(portBytes[0]) << 8) |
                                                static_cast<uint16_t>(portBytes[1]));

    int32_t nextState = packet.varInt();

    std::cout << "Protocol: " << protocolVersion << "\nNext State: " << nextState
              << "\n address: " << address << ":" << port << "\n";

    switch (nextState) {
    case 1:
      client.updateState(castle::protocol::State::Status);
      break;
    case 2:
      client.updateState(castle::protocol::State::Login);
      break;
    default:
      break;
    }
  }
} // namespace castle::handlers
