#include "handlers/status.hpp"
#include "client.hpp"
#include "protocol/protocol.hpp"
#include "tcp.hpp"
#include <cstdint>
#include <iostream>
#include <vector>

namespace {
void handleStatusRequest(Client &client) {

	const std::string json = R"({
        "version": {
            "name": "My C++ Server",
            "protocol": 777
        },
        "players": {
            "max": 69,
            "online": 420,
            "sample": [
      {
        "name": "kronborg",
        "id": "82f59091-e5e2-4638-b6d8-4108c7851b95"
      }
            ]
        },
        "description": {
            "text": "Hello from C++!"
        }
    })";

	std::vector<uint8_t> response;

	minecraft::protocol::writeString(json, response);

	minecraft::protocol::sendPacket(client.connection(), 0x00, response);
}
void handlePingRequest(Client &client) {
	TcpConnection &connection = client.connection();

	uint8_t payload[8];

	connection.readAll(payload, sizeof(payload));

	minecraft::protocol::sendPacket(connection, 0x01, payload);
}
} // namespace

namespace minecraft::protocol::handlers {

void handleStatus(Client &client) {

	TcpConnection &connection = client.connection();

	minecraft::protocol::readVarInt(connection);
	int32_t packetId = minecraft::protocol::readVarInt(connection);
	switch (packetId) {
	case 0x00:
		handleStatusRequest(client);
		break;
	case 0x01:
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
