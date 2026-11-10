#pragma once

#include "castle/client.hpp"
#include "castle/world/chunk.hpp"
#include "castle/world/chunkPostion.hpp"
#include <memory>
#include <mutex>
#include <thread>
#include <unordered_set>
#include <vector>
namespace castle::server {

  class Server {
    public:
    private:
      std::unordered_set<world::Chunk, world::ChunkPosition, world::ChunkPositionHash> chunks;
      std::vector<std::shared_ptr<castle::Client>> _clients;

      std::mutex _clientMutex;

      std::vector<std::jthread> _workers;
  };
} // namespace castle::server
