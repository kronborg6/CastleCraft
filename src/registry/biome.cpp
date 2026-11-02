#include "registry/biome.hpp"
#include "protocol/protocol.hpp"
#include "tcp.hpp"
#include <cstdint>
#include <vector>
namespace castle::registry {
  void biomeRegistry(TcpConnection& connection) {
    std::vector<std::uint8_t> data;

    minecraft::protocol::writeString("minecraft:worldgen/biome", data);

    minecraft::protocol::writeVarInt(BIOME_TYPES.size(), data);

    for (const auto& name : BIOME_TYPES) {

      minecraft::protocol::writeString(name, data);
      data.push_back(0x00);
    }
    minecraft::protocol::sendPacket(connection, 0x07, data);
  }
} // namespace castle::registry
