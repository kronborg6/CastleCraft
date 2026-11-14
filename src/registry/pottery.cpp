#include "castle/registry/pottery.hpp"
#include "castle/network/packet_send.hpp"
#include "castle/network/tcp_connection.hpp"
#include "castleProtocol/protocol/configuration.hpp"
#include <cstdint>
#include <vector>
namespace castle::registry {
  using ConfigurationPacket =
      castle::protocol::PacketWriter<castle::protocol::ConfigurationClientbound>;

  void potteryEntrie(castle::network::TcpConnection& connection) {
    constexpr std::array patterns = {
        "minecraft:angler",     "minecraft:archer", "minecraft:arms_up", "minecraft:blade",
        "minecraft:brewer",     "minecraft:burn",   "minecraft:danger",  "minecraft:explorer",
        "minecraft:flow",       "minecraft:friend", "minecraft:guster",  "minecraft:heart",
        "minecraft:heartbreak", "minecraft:howl",   "minecraft:miner",   "minecraft:mourner",
        "minecraft:plenty",     "minecraft:prize",  "minecraft:scrape",  "minecraft:sheaf",
        "minecraft:shelter",    "minecraft:skull",  "minecraft:snort",
    };
    // Registry identifier
    ConfigurationPacket packet(castle::protocol::ConfigurationClientbound::RegistryData);

    packet.addString("minecraft:decorated_pot_pattern");

    // Number of entries
    packet.addVarInt(static_cast<std::int32_t>(patterns.size()));

    for (const auto& name : patterns) {

      packet.addString(name);

      packet.addByte(0x00);
    }

    castle::network::sendPacket(connection, packet);
  }

} // namespace castle::registry
