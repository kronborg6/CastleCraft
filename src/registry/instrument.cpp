#include "registry/instrument.hpp"
#include "protocol/protocol.hpp"
#include "tcp.hpp"
#include <algorithm>
#include <cstdint>
#include <stdexcept>
#include <vector>

namespace castle::registry {

  void instrumentRegistry(TcpConnection& connection) {
    std::vector<std::uint8_t> data;

    minecraft::protocol::writeString("minecraft:instrument", data);
    minecraft::protocol::writeVarInt(static_cast<std::int32_t>(INSTRUMENT_TYPES.size()), data);

    for (const auto& name : INSTRUMENT_TYPES) {
      minecraft::protocol::writeString(name, data);
      data.push_back(0x00);
    }

    minecraft::protocol::sendPacket(connection, 0x07, data);
  }
  std::uint16_t instrumentTypeId(std::string_view name) {

    auto it = std::find_if(INSTRUMENT_TYPES.begin(),
                           INSTRUMENT_TYPES.end(),
                           [&](const std::string& d) { return d == name; });

    if (it == INSTRUMENT_TYPES.end()) {
      throw std::runtime_error("unknown damge type: " + std::string(name));
    }

    return static_cast<std::uint16_t>(std::distance(INSTRUMENT_TYPES.begin(), it));
  }
} // namespace castle::registry
