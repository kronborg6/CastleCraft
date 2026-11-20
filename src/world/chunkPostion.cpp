#include "castle/world/chunkPostion.hpp"
#include "castleProtocol/endian.hpp"
#include <cmath>

namespace castle::world {

  ChunkPosition getChunkFromPostion(const float& x, const float& z) {
    ChunkPosition cPostion;

    float g = x / 16;
    float e = z / 16;

    cPostion.x = static_cast<std::int32_t>(std::floor((x / 16.0f)));
    cPostion.z = static_cast<std::int32_t>(std::floor((z / 16.0f)));

    return cPostion;
  }
  ChunkPosition getChunkFromPostion(const type::Postion& postion) {
    ChunkPosition cPostion;

    cPostion.x = static_cast<std::int32_t>(std::floor(static_cast<float>(postion.x) / 16.0f));
    cPostion.z = static_cast<std::int32_t>(std::floor(static_cast<float>(postion.z) / 16.0f));

    return cPostion;
  }

} // namespace castle::world
