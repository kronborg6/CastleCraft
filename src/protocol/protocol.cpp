#include "protocol/protocol.hpp"
#include "client.hpp"
#include "handlers/configuration.hpp"
#include "handlers/handshake.hpp"
#include "handlers/login.hpp"
#include "handlers/status.hpp"
#include "protocol/state.hpp"
#include "tcp.hpp"
#include <arpa/inet.h>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <optional>
#include <span>
#include <stdexcept>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

namespace minecraft::protocol {

int32_t readVarInt(TcpConnection &connection) {
	int32_t value = 0;
	int position = 0;

	while (true) {
		const uint8_t byte = connection.readByte();

		value |= (byte & 0x7F) << position;

		if ((byte & 0x80) == 0) {
			break;
		}

		position += 7;

		if (position >= 32) {
			throw std::runtime_error("VarInt too large");
		}
	}

	return value;
}

void writeVarInt(int32_t value, std::vector<uint8_t> &output) {
	while (true) {
		if ((value & ~0x7F) == 0) {
			output.push_back(static_cast<uint8_t>(value));
			return;
		}

		output.push_back(static_cast<uint8_t>((value & 0x7F) | 0x80));

		value = static_cast<uint32_t>(value) >> 7;
	}
}

std::string readString(TcpConnection &connection) {
	int32_t length = readVarInt(connection);

	std::string result(static_cast<std::size_t>(length), '\0');

	connection.readAll(result.data(), result.length());

	return result;
}

void writeString(const std::string &text, std::vector<uint8_t> &output) {
	writeVarInt(static_cast<int32_t>(text.size()), output);

	output.insert(output.end(), text.begin(), text.end());
}

void sendPacket(TcpConnection &connection, int32_t packetId,
                const std::span<uint8_t> &data) {
	std::vector<uint8_t> packet;

	writeVarInt(packetId, packet);

	packet.insert(packet.end(), data.begin(), data.end());

	std::vector<uint8_t> framed;

	writeVarInt(static_cast<int32_t>(packet.size()), framed);

	framed.insert(framed.end(), packet.begin(), packet.end());

	connection.writeAll(framed.data(), framed.size());
}

std::optional<State> toState(int32_t value) {
	switch (value) {
	case 0:
		return State::Handshake;
	case 1:
		return State::Status;
	case 2:
		return State::Login;
	case 3:
		return State::Configuration;
	case 4:
		return State::Play;
	default:
		return std::nullopt;
	}
}

void handleClient(Client &client) {
	while (client.isConnected()) {
		try {

			switch (client.getState()) {
			case State::Handshake:
				std::cout << "Started handshake\n";
				handlers::handleHandshake(client);
				break;
			case State::Status:
				std::cout << "Started status state\n";
				handlers::handleStatus(client);
				break;
			case State::Login:
				// std::cout << "Started login state\n";
				handlers::handleLogin(client);
				break;
			case State::Configuration:
				// std::cout << "Started configuration state\n";
				handlers::handleConfiguration(client);
				break;
			case State::Play:
				std::cout << "Started play state\n";
				// handlePlay(client);
				break;
			}
		} catch (const std::exception &e) {
			if (client.getState() == State::Configuration ||
			    client.getState() == State::Play) {
				std::cout << client.profile.username << ": " << e.what()
				          << '\n';
			} else {
				std::cout << "Client disconnected: " << e.what() << '\n';
			}
			client.disconected();
		}
	}
}
} // namespace minecraft::protocol
