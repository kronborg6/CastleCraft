#include "registry/pottery.hpp"
#include "castle/network/tcp_connection.hpp"
#include "protocol/protocol.hpp"
#include <cstdint>
#include <vector>
namespace castle::registry {
  void potteryEntrie(csatle::network::TcpConnection& connection) {
    std::vector<std::uint8_t> output;
    constexpr std::array patterns = {
        "minecraft:angler",     "minecraft:archer", "minecraft:arms_up", "minecraft:blade",
        "minecraft:brewer",     "minecraft:burn",   "minecraft:danger",  "minecraft:explorer",
        "minecraft:flow",       "minecraft:friend", "minecraft:guster",  "minecraft:heart",
        "minecraft:heartbreak", "minecraft:howl",   "minecraft:miner",   "minecraft:mourner",
        "minecraft:plenty",     "minecraft:prize",  "minecraft:scrape",  "minecraft:sheaf",
        "minecraft:shelter",    "minecraft:skull",  "minecraft:snort",
    };
    // Registry identifier
    minecraft::protocol::writeString("minecraft:decorated_pot_pattern", output);

    // Number of entries
    minecraft::protocol::writeVarInt(patterns.size(), output);

    for (const auto& name : patterns) {

      minecraft::protocol::writeString(name, output);
      output.push_back(0x00);
    }

    minecraft::protocol::sendPacket(connection, 0x07, output);
  }

  void potteryEntrie(std::vector<std::uint8_t> output) {
    constexpr std::array patterns = {
        "minecraft:angler",     "minecraft:archer", "minecraft:arms_up", "minecraft:blade",
        "minecraft:brewer",     "minecraft:burn",   "minecraft:danger",  "minecraft:explorer",
        "minecraft:flow",       "minecraft:friend", "minecraft:guster",  "minecraft:heart",
        "minecraft:heartbreak", "minecraft:howl",   "minecraft:miner",   "minecraft:mourner",
        "minecraft:plenty",     "minecraft:prize",  "minecraft:scrape",  "minecraft:sheaf",
        "minecraft:shelter",    "minecraft:skull",  "minecraft:snort",
    };
    // Registry identifier
    minecraft::protocol::writeString("minecraft:decorated_pot_pattern", output);

    // Number of entries
    minecraft::protocol::writeVarInt(1, output);

    for (const auto& name : patterns) {

      minecraft::protocol::writeString(name, output);
      output.push_back(0x00);
    }
  }
} // namespace castle::registry
