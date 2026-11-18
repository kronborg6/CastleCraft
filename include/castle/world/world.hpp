#pragma once

#include "castle/world/chunk.hpp"
#include "castle/world/chunkPostion.hpp"
#include "castle/world/postion.hpp"
#include "castle/world/woldGreanter.hpp"
#include <atomic>
#include <cstdint>
#include <unordered_map>
#include <uuid/uuid.h>
namespace castle::world {
  class World {
      inline static std::atomic<std::uint64_t> _nextId{1};

    public:
      World() : _id(_nextId.fetch_add(1)) {
      }

      std::uint64_t id() const {
        return _id;
      }

      const world::Chunk& GetChunk(world::ChunkPosition postion);

      bool placeBlock(Posttion postion) {
        return false;
      }

      bool removeBlock(Posttion postion) {
        return false;
      }

    private:
      std::uint64_t _id;
      std::string _name;
      std::unordered_map<ChunkPosition, Chunk, ChunkPositionHash> _chunks;

      std::mutex _chunkLock;
  };
} // namespace castle::world
