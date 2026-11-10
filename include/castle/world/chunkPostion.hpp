#pragma once

#include <cstdint>
#include <functional>
namespace castle::world {
  struct ChunkPosition {
      std::int32_t x = 0;
      std::int32_t z = 0;

      bool operator==(const ChunkPosition&) const = default;
  };

  struct ChunkPositionHash {
      std::size_t operator()(const ChunkPosition& pos) const noexcept {
        std::size_t h1 = std::hash<std::int32_t>{}(pos.x);
        std::size_t h2 = std::hash<std::int32_t>{}(pos.z);

        return h1 ^ (h2 << 1);
      };
  };

}; // namespace castle::world
