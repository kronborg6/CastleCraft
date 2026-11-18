#include "castle/world/world.hpp"

namespace castle::world {

  const world::Chunk& World::GetChunk(world::ChunkPosition postion) {
    std::lock_guard<std::mutex> lock(_chunkLock);

    auto it = _chunks.find(postion);

    if (it != _chunks.end()) {
      return it->second;
    }

    auto [newIt, inserted] = _chunks.emplace(postion, world::generateStoneChunk(postion));

    return newIt->second;
  }
} // namespace castle::world
