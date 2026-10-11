#include "handlers/handshake.hpp"
#include "protocol/protocol.hpp"
#include "protocol/state.hpp"
#include "tcp.hpp"
#include <cstdint>
#include <iostream>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

namespace minecraft::protocol::handlers {
void handleHandshake(Client &client) {
	TcpConnection &connection = client.connection();

	int32_t packetLength = minecraft::protocol::readVarInt(connection);
	int32_t packetId = minecraft::protocol::readVarInt(connection);

	if (packetId != 0) {
		return;
	}

	int32_t protocolVersion = minecraft::protocol::readVarInt(connection);
	std::string address = minecraft::protocol::readString(connection);

	uint8_t portBytes[2];

	connection.readAll(portBytes, 2);

	const uint16_t port =
	    static_cast<uint16_t>((static_cast<uint16_t>(portBytes[0]) << 8) |
	                          static_cast<uint16_t>(portBytes[1]));

	int32_t nextState = minecraft::protocol::readVarInt(connection);

	std::cout << "Protocol: " << protocolVersion
	          << "\nNext State: " << nextState << "\n address: " << address
	          << ":" << port << "\n";

	switch (nextState) {
	case 1:
		client.updateState(minecraft::protocol::State::Status);
		break;
	case 2:
		client.updateState(minecraft::protocol::State::Login);
		break;
	default:
		break;
	}
}
} // namespace minecraft::protocol::handlers
