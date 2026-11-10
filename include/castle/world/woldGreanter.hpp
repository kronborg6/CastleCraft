#pragma once

#include "castle/world/chunk.hpp"
#include "castleProtocol/package/writePacket.hpp"
#include "castleProtocol/protocol/play.hpp"
namespace castle::world {
  using ChunkPacketWriter = castle::protocol::PacketWriter<castle::protocol::PlayClientbound>;

  constexpr std::int32_t MIN_Y = -64;
  constexpr std::int32_t WORLD_HEIGHT = 384;
  constexpr std::int32_t GROUND_Y = 63;
  constexpr std::int32_t STONE_STATE_ID = 1;
  constexpr std::int32_t AIR_STATE_ID = 0;
  constexpr std::int32_t PLAINS_BIOME_ID = 0;

  void writeHeightmaps(ChunkPacketWriter& packet, const Chunk& chunk);
  void writeSections(ChunkPacketWriter& packet, const Chunk& chunk);
  void writeLightData(ChunkPacketWriter& packet, const Chunk& chunk);

  Chunk generateStoneChunk(ChunkPosition position);
  ChunkPacketWriter encodeChunk(const Chunk& chunk);
} // namespace castle::world
