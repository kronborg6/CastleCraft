#include "entries/entries.hpp"
#include "castle/network/tcp_connection.hpp"
#include "entries/banner.hpp"
#include "entries/damge_types.hpp"
#include "protocol/protocol.hpp"
#include "registry/damge_types.hpp"
#include <cstdint>
#include <vector>
namespace castle::entries {

  void entriesTypes(csatle::network::TcpConnection& connection) {
    std::vector<std::uint8_t> output;
    minecraft::protocol::writeVarInt(3, output);

    minecraft::protocol::writeString("minecraft:damage_type", output);

    // Number of tags
    minecraft::protocol::writeVarInt(3, output);

    explosionTypesEntrie(output);
    fireTypesEntrie(output);
    bypassShieldTypesEntrie(output);

    minecraft::protocol::writeString("minecraft:timeline", output);

    // Number of tags
    minecraft::protocol::writeVarInt(2, output);

    // Tag 0: minecraft:universal
    minecraft::protocol::writeString("minecraft:universal", output);
    minecraft::protocol::writeVarInt(1, output);
    minecraft::protocol::writeVarInt(3, output);

    // Tag 1: minecraft:in_overworld
    minecraft::protocol::writeString("minecraft:in_overworld", output);
    minecraft::protocol::writeVarInt(4, output);

    for (std::int32_t id = 0; id < 4; ++id) {
      minecraft::protocol::writeVarInt(id, output);
    }

    bannerTagsEntrie(output);

    // Send Update Tags
    minecraft::protocol::sendPacket(connection, 0x0E, output);
  }
} // namespace castle::entries
