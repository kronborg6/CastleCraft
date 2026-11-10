#pragma once

#include "castle/world/chunkPostion.hpp"
#include "castle/world/chunkSection.hpp"
#include "castleProtocol/package/writePacket.hpp"
#include "castleProtocol/protocol/play.hpp"
#include <cstdint>
#include <vector>

namespace castle::world {

  struct Chunk {
      static constexpr std::int32_t WIDTH = 16;
      static constexpr std::int32_t DEPTH = 16;

      ChunkPosition position;
      std::vector<ChunkSection> sections;
  };

  castle::protocol::PacketWriter<castle::protocol::PlayClientbound> chunk(std::int32_t x,
                                                                          std::int32_t z);
} // namespace castle::world
