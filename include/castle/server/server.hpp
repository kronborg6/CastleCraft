#pragma once

#include "castle/client.hpp"
#include "castle/world/chunk.hpp"
#include "castle/world/chunkPostion.hpp"
#include <unordered_set>
#include <vector>
namespace castle::server {

  class Server {
    public:
      Server() = default;
      Server(Server&&) = default;
      Server(const Server&) = default;
      Server& operator=(Server&&) = default;
      Server& operator=(const Server&) = default;
      ~Server() = default;

    private:
      std::unordered_set<world::Chunk, world::ChunkPosition, world::ChunkPositionHash> chunks;
      std::vector<castle::Client> _client;
  };
} // namespace castle::server
