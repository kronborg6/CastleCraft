
#include "client.hpp"
#include "protocol.hpp"
#include "tcp.hpp"
#include <cstdint>
#include <iostream>

namespace {

  void handleLoginRequst(Client& client) {
  }
} // namespace
namespace minecraft::protocol::handlers {
  void handleLogin(Client& client) {
    TcpConnection& connection = client.connection();
    int32_t length = minecraft::protocol::readVarInt(connection);
    int32_t packetId = minecraft::protocol::readVarInt(connection);

    std::cout << "length: " << length << "\nacketId: " << packetId << "\n";
  }
} // namespace minecraft::protocol::handlers
