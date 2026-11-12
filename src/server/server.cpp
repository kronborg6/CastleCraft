#include "castle/server/server.hpp"
#include "castle/client.hpp"
#include "castle/handlers/configuration.hpp"
#include "castle/handlers/handshake.hpp"
#include "castle/handlers/login.hpp"
#include "castle/handlers/play.hpp"
#include "castle/handlers/status.hpp"
#include "castle/nbt.hpp"
#include "castle/writer.hpp"
#include "castleProtocol/protocol/state.hpp"
#include <cstdio>
#include <mutex>
#include <sys/socket.h>

namespace castle::server {
  void Server::handleClient(Client& client) {
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
          castle::handlers::handleConfiguration(*this, client);
          break;
        case castle::protocol::State::Play:
          castle::handlers::handlePlayState(*this, client);
          break;
        }
      } catch (const std::exception& error) {
        if (client.getState() == castle::protocol::State::Configuration ||
            client.getState() == castle::protocol::State::Play) {
          std::cout << client.profile.username << ": " << error.what() << '\n';

          nbt::Nbt text = nbt::Nbt::makeCompound("");

          std::string msg = std::format("{} left CastleCraft", client.profile.username);
          text.add(nbt::Nbt::makeString("text", msg));
          text.add(nbt::Nbt::makeString("color", "yellow"));

          handlers::PlayPacketWriter boardcastWellom(protocol::PlayClientbound::SystemChatMessage);

          boardcastWellom.addBytes(nbt::NbtWriter::writer(text, {.namelessRoot = true}));
          boardcastWellom.addByte(0x00);

          client.broadcast(boardcastWellom, false);

        } else {
          std::cout << ": " << error.what() << '\n';
        }
        client.disconected();
      }
    }
  }

  void Server::addClient(std::shared_ptr<Client> client) {
    std::lock_guard<std::mutex> lock(_clientMutex);
    _clients.push_back(std::move(client));
  }

  void Server::removeClient(const Client& client) {
    std::lock_guard<std::mutex> lock(_clientMutex);

    std::erase_if(_clients, [&](const auto& current) { return current.get() == &client; });
  }

  void Server::run() {
    _serverRunning = true;
    while (_serverRunning) {
      int clientSocket = accept(_serverSocket, nullptr, nullptr);

      if (clientSocket < 0) {
        continue;
      }
      auto client = std::make_shared<castle::Client>(
          clientSocket, _idCount++, [this](const auto& packet, const Client* exclude) {
            broadcast(packet, exclude);
          });

      addClient(client);

      _workers.emplace_back([this, client]() {
        handleClient(*client);
        removeClient(*client);
      });
    }
  }
} // namespace castle::server
