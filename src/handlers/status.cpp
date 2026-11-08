#include "handlers/status.hpp"
#include "castle/client.hpp"
#include "castle/network/packet_receiver.hpp"
#include "castle/network/packet_send.hpp"
#include "castle/network/tcp_connection.hpp"
#include "castleProtocol/package/readPackage.hpp"
#include "castleProtocol/package/writePacket.hpp"
#include "castleProtocol/protocol/status.hpp"
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

	castle::protocol::PacketWriter<castle::protocol::StatusClientbound> packet(
	    castle::protocol::StatusClientbound::StatusResponse);

	packet.addString(json);

	castle::network::sendPacket(client.connection(), packet);
}
void handlePingRequest(
    Client &client,
    castle::protocol::PacketReader<castle::protocol::StatusServerbound>
        &packet) {
	csatle::network::TcpConnection &connection = client.connection();

	auto payload = packet.bytes(8);

	castle::protocol::PacketWriter<castle::protocol::StatusClientbound>
	    writePacket(castle::protocol::StatusClientbound::PongResponse);

	writePacket.addBytes(payload);

	castle::network::sendPacket(connection, writePacket);
}
} // namespace

using namespace minecraft::protocol;
namespace minecraft::protocol::handlers {

void handleStatus(Client &client) {

	csatle::network::TcpConnection &connection = client.connection();

	std::vector<std::uint8_t> data = castle::network::receivePacket(connection);

	castle::protocol::PacketReader<castle::protocol::StatusServerbound> packet(
	    data);

	switch (packet.PacketId) {
	case castle::protocol::StatusServerbound::StatusRequest:
		handleStatusRequest(client);
		break;
	case castle::protocol::StatusServerbound::PingRequest:
		handlePingRequest(client, packet);
		break;
	default:
		std::cerr << "Unknown packet in Status state: "
		          << static_cast<std::int32_t>(packet.PacketId) << '\n';
		break;
	}
	// implementation
}

} // namespace minecraft::protocol::handlers
