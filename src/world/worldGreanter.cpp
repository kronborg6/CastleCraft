#include "castle/network/packet_send.hpp"
#include "castle/network/tcp_connection.hpp"
#include "castle/world/chunkPostion.hpp"
#include "castle/world/woldGreanter.hpp"
#include <optional>
namespace castle::world {
  std::vector<std::uint64_t> packHeightmap(const std::array<std::uint16_t, 256>& heights) {
    constexpr std::size_t BITS = 9;
    constexpr std::size_t VALUES_PER_LONG = 64 / BITS;

    std::vector<std::uint64_t> packed((heights.size() + VALUES_PER_LONG - 1) / VALUES_PER_LONG, 0);

    for (std::size_t i = 0; i < heights.size(); ++i) {
      const std::size_t longIndex = i / VALUES_PER_LONG;
      const std::size_t bitOffset = (i % VALUES_PER_LONG) * BITS;

      packed[longIndex] |= static_cast<std::uint64_t>(heights[i]) << bitOffset;
    }

    return packed;
  }
  void writeHeightmaps(ChunkPacketWriter& packet, const Chunk& chunk) {
    std::array<std::uint16_t, 256> heights{};

    for (std::int32_t z = 0; z < 16; ++z) {
      for (std::int32_t x = 0; x < 16; ++x) {
        std::uint16_t height = 0;

        for (std::size_t s = chunk.sections.size(); s-- > 0;) {

          const auto& section = chunk.sections[s];

          bool found = false;

          for (std::int32_t y = 15; y >= 0; --y) {
            const std::size_t index = static_cast<std::size_t>((y * 16 + z) * 16 + x);

            if (section.blocks[index] != AIR_STATE_ID) {
              height = static_cast<std::uint16_t>(s * 16 + y + 1);
              found = true;
              break;
            }
          }

          if (found) {
            break;
          }
        }

        heights[static_cast<std::size_t>(z * 16 + x)] = height;
      }
    }

    const auto packed = packHeightmap(heights);

    // Three client heightmap types for this protocol version.
    constexpr std::array<std::int32_t, 3> types = {
        1, // WORLD_SURFACE
        4, // MOTION_BLOCKING
        5  // MOTION_BLOCKING_NO_LEAVES
    };

    packet.addVarInt(static_cast<std::int32_t>(types.size()));

    for (const auto type : types) {
      packet.addVarInt(type);
      packet.addVarInt(static_cast<std::int32_t>(packed.size()));

      for (const auto value : packed) {
        packet.addInt<std::uint64_t>(value);
      }
    }
  }
  void writeBlockStates(std::vector<std::uint8_t>& data, const ChunkSection& section) {
    const auto first = section.blocks[0];

    // Find whether another block state exists.
    std::optional<std::uint32_t> second;

    for (const auto block : section.blocks) {
      if (block != first) {
        if (second && block != *second) {
          throw std::runtime_error("More than two block states");
        }

        second = block;
      }
    }

    if (!second) {
      // Single-valued palette
      data.push_back(0);
      castle::protocol::writeVarInt(static_cast<std::int32_t>(first), data);

      return;
    }

    // Two-valued palette: minimum 4 bits per entry.
    data.push_back(4);

    castle::protocol::writeVarInt(2, data);
    castle::protocol::writeVarInt(static_cast<std::int32_t>(first), data);
    castle::protocol::writeVarInt(static_cast<std::int32_t>(*second), data);

    // 4096 blocks / 16 entries per long = 256 longs.
    for (std::size_t i = 0; i < 4096; i += 16) {
      std::uint64_t packed = 0;

      for (std::size_t j = 0; j < 16; ++j) {
        const std::uint64_t paletteIndex = section.blocks[i + j] == first ? 0 : 1;

        packed |= paletteIndex << (j * 4);
      }

      castle::endian::writeBigEndian<std::uint64_t>(packed, data);
    }
  }
  void writeLightMask(ChunkPacketWriter& packet, std::uint32_t mask) {
    std::array<std::uint8_t, 4> bytes{};
    std::int32_t count = 0;

    while (mask != 0) {
      bytes[static_cast<std::size_t>(count++)] = static_cast<std::uint8_t>(mask & 0xFF);

      mask >>= 8;
    }

    packet.addVarInt(count);

    for (std::int32_t i = 0; i < count; ++i) {
      packet.addByte(bytes[static_cast<std::size_t>(i)]);
    }
  }

  void writeLightData(ChunkPacketWriter& packet, const Chunk& chunk) {
    const std::size_t lightSectionCount = chunk.sections.size() + 2;

    if (lightSectionCount > 64) {
      throw std::runtime_error("Light mask requires multiple longs");
    }

    const std::uint32_t allSections = (1U << 26) - 1U;

    // Sky light: data for all 26 positions
    writeLightMask(packet, allSections);

    // Block light: no light-emitting blocks
    writeLightMask(packet, 0);

    // Empty sky light mask: none
    writeLightMask(packet, 0);

    // Empty block light mask: all positions
    writeLightMask(packet, allSections);

    // Number of sky light arrays
    packet.addVarInt(static_cast<std::int32_t>(lightSectionCount));

    for (std::size_t s = 0; s < lightSectionCount; ++s) {
      std::array<std::uint8_t, 2048> skyLight{};

      const std::int32_t sectionY = MIN_Y + (static_cast<std::int32_t>(s) - 1) * 16;

      for (std::int32_t y = 0; y < 16; ++y) {
        const std::int32_t worldY = sectionY + y;

        const std::uint8_t level = worldY > GROUND_Y ? 15 : 0;

        for (std::int32_t z = 0; z < 16; ++z) {
          for (std::int32_t x = 0; x < 16; ++x) {
            const std::size_t index = static_cast<std::size_t>((y * 16 + z) * 16 + x);

            const std::size_t byteIndex = index / 2;
            const std::size_t shift = (index % 2) * 4;

            skyLight[byteIndex] |= static_cast<std::uint8_t>(level << shift);
          }
        }
      }

      packet.addVarInt(static_cast<std::int32_t>(skyLight.size()));

      packet.addBytes(skyLight);
    }

    // No block light arrays
    packet.addVarInt(0);
  }
  void writeSections(ChunkPacketWriter& packet, const Chunk& chunk) {
    std::vector<std::uint8_t> data;

    for (const auto& section : chunk.sections) {
      std::int16_t nonAirCount = 0;

      for (const auto block : section.blocks) {
        if (block != AIR_STATE_ID) {
          ++nonAirCount;
        }
      }

      // Non-air block count
      castle::endian::writeBigEndian<std::int16_t>(nonAirCount, data);

      // Fluid count (always zero in our stone world)
      castle::endian::writeBigEndian<std::int16_t>(0, data);

      writeBlockStates(data, section);

      // Biomes: all plains
      data.push_back(0); // Single-valued palette
      castle::protocol::writeVarInt(static_cast<std::int32_t>(section.biomes[0]), data);
    }

    packet.addVarInt(static_cast<std::int32_t>(data.size()));

    packet.addBytes(data);
  }

  constexpr std::size_t SECTION_COUNT = WORLD_HEIGHT / ChunkSection::HEIGHT;

  Chunk generateStoneChunk(ChunkPosition position) {
    Chunk chunk{};
    chunk.position = position;
    chunk.sections.resize(SECTION_COUNT);

    for (std::size_t i = 0; i < chunk.sections.size(); ++i) {
      auto& section = chunk.sections[i];

      for (std::int32_t y = 0; y < 16; ++y) {
        const std::int32_t worldY = MIN_Y + static_cast<std::int32_t>(i) * 16 + y;

        for (std::int32_t z = 0; z < 16; ++z) {
          for (std::int32_t x = 0; x < 16; ++x) {
            const std::size_t index = static_cast<std::size_t>((y * 16 + z) * 16 + x);

            section.blocks[index] = worldY <= GROUND_Y ? STONE_STATE_ID : AIR_STATE_ID;
          }
        }
      }

      section.biomes.fill(PLAINS_BIOME_ID);
    }

    return chunk;
  }
  ChunkPacketWriter encodeChunk(const Chunk& chunk) {
    ChunkPacketWriter packet(castle::protocol::PlayClientbound::ChunkDataAndUpdateLight);

    packet.addInt<std::int32_t>(chunk.position.x);
    packet.addInt<std::int32_t>(chunk.position.z);

    writeHeightmaps(packet, chunk);
    writeSections(packet, chunk);

    // No block entities
    packet.addVarInt(0);

    writeLightData(packet, chunk);

    return packet;
  }
} // namespace castle::world
