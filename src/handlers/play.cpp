#include "handlers/play.hpp"
#include "client.hpp"
#include "protocol/protocol.hpp"
#include "tcp.hpp"
#include <iostream>
#include <vector>

namespace minecraft::protocol::handlers {
  void initialiizePlay(Client& clinet) {
    TcpConnection& connection = clinet.connection();
    std::vector<uint8_t> data;

    minecraft::protocol::sendPacket(connection, 0x32, {});
  }
  void handlePlayState(Client& client) {

    TcpConnection& connection = client.connection();

    std::cout << "hello\n";
    std::cout << "hello\n";
    std::cout << "hello\n";
    const int32_t packetLength = minecraft::protocol::readVarInt(connection);

    const int32_t packetId = minecraft::protocol::readVarInt(connection);
    std::cout << packetLength << packetId;
    std::cout << packetLength << packetId;
    std::cout << packetLength << packetId;
    std::cout << packetLength << packetId;

    switch (packetId) {
    case 0x32:
      std::cout << "hello\n";
    default:
      std::cerr << "Unknown Configuration packet: 0x" << std::hex << packetId << std::dec << '\n';
      break;
    }
  }
} // namespace minecraft::protocol::handlers
