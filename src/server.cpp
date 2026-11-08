#include "server.hpp"
#include "castleProtocol/protocol/state.hpp"
#include "handlers/configuration.hpp"
#include "handlers/handshake.hpp"
#include "handlers/login.hpp"
#include "handlers/play.hpp"
#include "handlers/status.hpp"
#include <exception>
#include <iostream>

namespace minecraft::protocol {
void handleClient(Client &client) {
	while (client.isConnected()) {
		try {
			client.connectionCheck();

			switch (client.getState()) {
			case castle::protocol::State::Handshake:
				handlers::handleHandshake(client);
				break;
			case castle::protocol::State::Status:
				handlers::handleStatus(client);
				break;
			case castle::protocol::State::Login:
				handlers::handleLogin(client);
				break;
			case castle::protocol::State::Configuration:
				handlers::handleConfiguration(client);
				break;
			case castle::protocol::State::Play:
				handlers::handlePlayState(client);
				break;
			}
		} catch (const std::exception &error) {
			if (client.getState() == castle::protocol::State::Configuration ||
			    client.getState() == castle::protocol::State::Play) {
				std::cout << client.profile.username << ": " << error.what()
				          << '\n';
			} else {
				std::cout << "Client disconnected: " << error.what() << '\n';
			}
			client.disconected();
		}
	}
}
} // namespace minecraft::protocol
