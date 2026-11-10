#pragma once

#include "castle/world/chunk.hpp"
#include <array>
#include <cstdint>

namespace castle::world {
  struct ChunkSection {
      static constexpr std::int32_t WIDTH = 16;
      static constexpr std::int32_t HEIGHT = 16;
      static constexpr std::int32_t DEPTH = 16;
      static constexpr std::size_t BLOCK_COUNT = WIDTH * HEIGHT * DEPTH;
      static constexpr std::size_t BIOME_COUNT = 64;

      std::array<std::uint32_t, BLOCK_COUNT> blocks{};
      std::array<std::uint32_t, BIOME_COUNT> biomes{};
  };
} // namespace castle::world
