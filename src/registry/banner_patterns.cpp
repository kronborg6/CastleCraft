#include "castle/registry/banner_patterns.hpp"
#include "castle/network/packet_send.hpp"
#include "castle/network/tcp_connection.hpp"
#include "castleProtocol/protocol/configuration.hpp"
#include <algorithm>
#include <cstdint>
#include <stdexcept>
#include <string>
namespace castle::registry {
  using ConfigurationPacket =
      castle::protocol::PacketWriter<castle::protocol::ConfigurationClientbound>;

  void BannerPatternRegistry(castle::network::TcpConnection& connection) {

    ConfigurationPacket packet(castle::protocol::ConfigurationClientbound::RegistryData);

    packet.addString("minecraft:banner_pattern");

    packet.addVarInt(static_cast<std::int32_t>(registry::BANNER_PATTERS.size()));

    for (const auto& name : registry::BANNER_PATTERS) {

      packet.addString(name);

      packet.addByte(0x00);
    }

    castle::network::sendPacket(connection, packet);
  }
  std::uint16_t bannerPatternId(std::string_view name) {
    auto it = std::find_if(BANNER_PATTERS.begin(), BANNER_PATTERS.end(), [&](const std::string& d) {
      return d == name;
    });

    if (it == BANNER_PATTERS.end()) {
      throw std::runtime_error("unknown damge type: " + std::string(name));
    }

    return static_cast<std::uint16_t>(std::distance(BANNER_PATTERS.begin(), it));
  }
} // namespace castle::registry
