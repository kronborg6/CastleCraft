#include "entries/jukebox.hpp"
#include "protocol/protocol.hpp"
#include "protocol/state.hpp"
#include "tcp.hpp"

namespace castle::entries {
  void jukeboxEntrie(TcpConnection& connection) {
    std::vector<std::uint8_t> output;
    constexpr std::array variants = {
        "minecraft:11",        "minecraft:13",           "minecraft:5",
        "minecraft:blocks",    "minecraft:bounce",       "minecraft:cat",
        "minecraft:chirp",     "minecraft:creator",      "minecraft:creator_music_box",
        "minecraft:far",       "minecraft:lava_chicken", "minecraft:mall",
        "minecraft:mellohi",   "minecraft:otherside",    "minecraft:pigstep",
        "minecraft:precipice", "minecraft:relic",        "minecraft:stal",
        "minecraft:strad",     "minecraft:tears",        "minecraft:wait",
        "minecraft:ward"};

    minecraft::protocol::writeString("minecraft:jukebox_song", output);

    // Number of entries
    minecraft::protocol::writeVarInt(variants.size(), output);

    for (const auto& name : variants) {
      minecraft::protocol::writeString(name, output);
      output.push_back(0x00);
    }

    minecraft::protocol::sendPacket(connection, 0x07, output);
  }
} // namespace castle::entries
