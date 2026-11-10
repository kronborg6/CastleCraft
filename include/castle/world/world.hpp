#pragma once

#include "castle/world/chunk.hpp"
#include "castle/world/chunkPostion.hpp"
#include <unordered_map>
namespace castle::world {
  class World {
    public:
      World() = default;
      World(World&&) = default;
      World(const World&) = default;
      World& operator=(World&&) = default;
      World& operator=(const World&) = default;
      ~World() = default;

    private:
      std::unordered_map<ChuckPostion, Chunk, ChunkPositionHash> chunks;
  };

} // namespace castle::world
