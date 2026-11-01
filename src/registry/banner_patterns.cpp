#include "registry/banner_patterns.hpp"
#include "protocol/protocol.hpp"
#include "tcp.hpp"
#include <algorithm>
#include <cstdint>
#include <stdexcept>
#include <string>
namespace castle::registry {

  void BannerPatternRegistry(TcpConnection& connection) {

    std::vector<uint8_t> data;
    minecraft::protocol::writeString("minecraft:banner_pattern", data);
    minecraft::protocol::writeVarInt(static_cast<std::int32_t>(registry::BANNER_PATTERS.size()),
                                     data);

    for (const auto& name : registry::BANNER_PATTERS) {
      minecraft::protocol::writeString(name, data);
      data.push_back(0x00);
    }

    minecraft::protocol::sendPacket(connection, 0x07, data);
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
