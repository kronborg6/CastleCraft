#include "server.hpp"
#include "castle/handlers/configuration.hpp"
#include "castle/handlers/handshake.hpp"
#include "castle/handlers/login.hpp"
#include "castle/handlers/play.hpp"
#include "castle/handlers/status.hpp"
#include "castleProtocol/protocol/state.hpp"
#include <exception>
#include <iostream>

namespace minecraft::protocol {
  void handleClient(castle::Client& client) {
    while (client.isConnected()) {
      try {
        client.connectionCheck();

        switch (client.getState()) {
        case castle::protocol::State::Handshake:
          castle::handlers::handleHandshake(client);
          break;
        case castle::protocol::State::Status:
          castle::handlers::handleStatus(client);
          break;
        case castle::protocol::State::Login:
          castle::handlers::handleLogin(client);
          break;
        case castle::protocol::State::Configuration:
          castle::handlers::handleConfiguration(client);
          break;
        case castle::protocol::State::Play:
          castle::handlers::handlePlayState(client);
          break;
        }
      } catch (const std::exception& error) {
        if (client.getState() == castle::protocol::State::Configuration ||
            client.getState() == castle::protocol::State::Play) {
          std::cout << client.profile.username << ": " << error.what() << '\n';
        } else {
          std::cout << "Client disconnected: " << error.what() << '\n';
        }
        client.disconected();
      }
    }
  }
} // namespace minecraft::protocol
