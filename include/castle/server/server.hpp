#pragma once

#include "castle/client.hpp"
#include "castle/network/packet_send.hpp"
#include "castle/world/chunk.hpp"
#include "castle/world/chunkPostion.hpp"
#include "castle/world/woldGreanter.hpp"
#include "castleProtocol/package/writePacket.hpp"
#include "castleProtocol/protocol/state.hpp"
#include <algorithm>
#include <cstdint>
#include <iterator>
#include <memory>
#include <mutex>
#include <thread>
#include <unordered_set>
#include <vector>
namespace castle::server {

  class Server {
    public:
      Server(int socket) : _serverSocket(socket) {
      }
      void run();
      void handleClient(castle::Client& client);
      void addClient(std::shared_ptr<Client> client);
      void removeClient(const Client& client);

      std::vector<std::shared_ptr<Client>> getClientsByStatus(protocol::State state) {
        std::lock_guard<std::mutex> lock(_clientMutex);

        std::vector<std::shared_ptr<Client>> result;

        std::ranges::copy_if(
            _clients, std::back_inserter(result), [&](const std::shared_ptr<Client>& c) {
              return c->getState() == state;
            });

        return result;
      }

      void broadcast(const protocol::PacketWriter<protocol::PlayClientbound>& packet,
                     const Client* exclude = nullptr) {
        auto clints = this->getClientsByStatus(protocol::State::Play);

        for (const auto& client : clints) {

          if (client.get() == exclude) {
            continue;
          }

          auto& connection = client->connection();
          network::sendPacket(connection, packet);
        }
      }

      std::vector<std::shared_ptr<Client>> getClients() {
        std::lock_guard<std::mutex> lock(_clientMutex);
        return _clients;
      }

      std::int32_t getMaxPlayerCount() const {
        return _maxPlayerCount;
      }

      std::int32_t getPlayerCount() const {
        return _playerCount;
      }

      const world::Chunk& GetChunk(world::ChunkPosition postion) {
        std::lock_guard<std::mutex> lock(_clientMutex);

        auto it = _chunks.find(postion);

        if (it != _chunks.end()) {
          return it->second;
        }

        auto [newIt, inserted] = _chunks.emplace(postion, world::generateStoneChunk(postion));

        return newIt->second;
      }

      void initWorld() {
        for (int z = -10; z <= 10; ++z) {
          for (int x = -10; x <= 10; ++x) {
            world::ChunkPosition postion = {x, z};
            _chunks[postion] = world::generateStoneChunk(postion);
          }
        }
      }

    private:
      int _serverSocket;
      bool _serverRunning;
      std::int32_t _playerCount = 0;
      std::int32_t _maxPlayerCount = 24;

      std::int32_t _idCount = 1;
      std::vector<std::shared_ptr<castle::Client>> _clients;
      std::vector<std::jthread> _workers;

      std::unordered_map<world::ChunkPosition, world::Chunk, world::ChunkPositionHash> _chunks;

      std::mutex _clientMutex;
  };
} // namespace castle::server
