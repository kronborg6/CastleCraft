#pragma once

#include "castle/client.hpp"
#include "castle/network/packet_send.hpp"
#include "castle/world/chunk.hpp"
#include "castle/world/chunkPostion.hpp"
#include "castle/world/woldGreanter.hpp"
#include "castle/world/world.hpp"
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

      void setBlock(type::Postion position, std::uint32_t blockId) {
        constexpr std::int32_t MIN_Y = -64;
        constexpr std::int32_t MAX_Y = 320;
        constexpr std::int32_t SIZE = 16;

        const auto x = static_cast<std::int32_t>(std::floor(position.x));
        const auto y = static_cast<std::int32_t>(std::floor(position.y));
        const auto z = static_cast<std::int32_t>(std::floor(position.z));

        if (y < MIN_Y || y >= MAX_Y) {
          return;
        }

        const auto chunkPosition = world::getChunkFromPostion(position);
        auto& chunk = _GetChunk({chunkPosition.x, chunkPosition.z});

        // Local block coordinates inside the chunk.
        const auto localX = x - chunkPosition.x * SIZE;
        const auto localZ = z - chunkPosition.z * SIZE;

        // Y relative to the bottom of the world.
        const auto relativeY = y - MIN_Y;

        const auto sectionIndex = relativeY / SIZE;
        const auto localY = relativeY % SIZE;

        const std::size_t index =
            static_cast<std::size_t>((localY * SIZE + localZ) * SIZE + localX);

        chunk.sections[static_cast<std::size_t>(sectionIndex)].blocks[index] = blockId;
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
      world::Chunk& _GetChunk(world::ChunkPosition postion) {
        std::lock_guard<std::mutex> lock(_clientMutex);

        auto it = _chunks.find(postion);

        if (it != _chunks.end()) {
          return it->second;
        }

        auto [newIt, inserted] = _chunks.emplace(postion, world::generateStoneChunk(postion));

        return newIt->second;
      }
      int _serverSocket;
      bool _serverRunning;
      std::int32_t _playerCount = 0;
      std::int32_t _maxPlayerCount = 24;

      std::int32_t _idCount = 1;
      std::vector<std::shared_ptr<castle::Client>> _clients;
      std::vector<std::jthread> _workers;
      world::World _world;

      std::unordered_map<world::ChunkPosition, world::Chunk, world::ChunkPositionHash> _chunks;

      std::mutex _clientMutex;
  };
} // namespace castle::server
