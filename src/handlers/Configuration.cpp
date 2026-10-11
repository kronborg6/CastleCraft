#include "handlers/Configuration.hpp"
#include "protocol.hpp"
#include "tcp.hpp"
#include <cstdint>
#include <iostream>

namespace minecraft::protocol::handlers {
  void handleConfiguration(Client& client) {
    TcpConnection& connection = client.connection();

    const int32_t length = minecraft::protocol::readVarInt(connection);
    const int32_t packetId = minecraft::protocol::readVarInt(connection);

    switch (packetId) {
    case 0x00:
      break;
    case 0x01:
      break;
    case 0x02:
      break;
    case 0x03:
      break;
    case 0x04:
      break;
    case 0x05:
      break;
    case 0x06:
      break;
    case 0x07:
      break;
    case 0x08:
      break;
    case 0x09:
    default:
      std::cerr << "Unknown Configuration packet: 0x" << std::hex << packetId << std::dec << '\n';
      break;
      break;
    }
  }
} // namespace minecraft::protocol::handlers
