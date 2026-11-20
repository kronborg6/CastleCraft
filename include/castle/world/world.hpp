#pragma once

#include "castle/world/chunk.hpp"
#include "castle/world/chunkPostion.hpp"
// #include "castle/world/postion.hpp"
#include "castle/world/woldGreanter.hpp"
#include "castleProtocol/types/postion.hpp"
#include <atomic>
#include <cmath>
#include <cstddef>
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

      bool removeBlock(type::Postion postion) {
        return false;
      }

      bool setBlock(type::Postion postion, std::uint32_t blockId) {
        auto chunkPostion = getChunkFromPostion(postion);

        auto& chunk = _GetChunk({chunkPostion.x, chunkPostion.z});

        const std::size_t index =
            static_cast<std::size_t>((postion.y * 16 + postion.z) * 16 + postion.x);

        chunk.sections[static_cast<std::size_t>(std::floor(static_cast<float>(postion.y) / 16.0f))]
            .blocks[index] = blockId;

        return true;
      }

    private:
      world::Chunk& _GetChunk(world::ChunkPosition postion) {
        std::lock_guard<std::mutex> lock(_chunkLock);

        auto it = _chunks.find(postion);

        if (it != _chunks.end()) {
          return it->second;
        }

        auto [newIt, inserted] = _chunks.emplace(postion, world::generateStoneChunk(postion));

        return newIt->second;
      }
      std::uint64_t _id;
      std::string _name;
      std::unordered_map<ChunkPosition, Chunk, ChunkPositionHash> _chunks;

      std::mutex _chunkLock;
  };
} // namespace castle::world
