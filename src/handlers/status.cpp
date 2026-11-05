#include "handlers/status.hpp"
#include "client.hpp"
#include "protocol/protocol.hpp"
#include "protocol/status.hpp"
#include "tcp.hpp"
#include <cstdint>
#include <iostream>
#include <vector>

namespace {
  void handleStatusRequest(Client& client) {

    const std::string json = R"({
        "version": {
            "name": "My C++ Server",
            "protocol": 777
        },
        "players": {
            "max": 69,
            "online": -420,
            "sample": [
      {
        "name": "kronborg",
        "id": "82f59091-e5e2-4638-b6d8-4108c7851b95"
      }
            ]
        },
        "description": {
            "text": "Hello to CastleCraft a minecraft written in C++!"
        }
    })";

    std::vector<uint8_t> response;

    minecraft::protocol::writeString(json, response);

    minecraft::protocol::sendPacket(
        client.connection(), minecraft::protocol::StatusServerbound::StatusRequest, response);
  }
  void handlePingRequest(Client& client) {
    TcpConnection& connection = client.connection();

    uint8_t payload[8];

    connection.readAll(payload, sizeof(payload));

    minecraft::protocol::sendPacket(
        connection, minecraft::protocol::StatusClientbound::PongResponse, payload);
  }
} // namespace

using namespace minecraft::protocol;
namespace minecraft::protocol::handlers {

  void handleStatus(Client& client) {

    TcpConnection& connection = client.connection();

    minecraft::protocol::readVarInt(connection);
    int32_t packetId = minecraft::protocol::readVarInt(connection);
    switch (static_cast<StatusServerbound>(packetId)) {
    case StatusServerbound::StatusRequest:
      handleStatusRequest(client);
      break;
    case StatusServerbound::PingRequest:
      std::cout << "we se ping\n";
      std::cout << "we se ping\n";
      std::cout << "we se ping\n";
      std::cout << "we se ping\n";
      std::cout << "we se ping\n";
      handlePingRequest(client);
      break;
    default:
      std::cerr << "Unknown packet in Status state: " << packetId << '\n';
      break;
    }
    // implementation
  }

} // namespace minecraft::protocol::handlers
