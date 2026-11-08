#include "castle/registry/instrument.hpp"
#include "castle/network/packet_send.hpp"
#include "castle/network/tcp_connection.hpp"
#include "castleProtocol/protocol/configuration.hpp"
#include <algorithm>
#include <cstdint>
#include <stdexcept>
#include <vector>

namespace castle::registry {
  using ConfigurationPacket =
      castle::protocol::PacketWriter<castle::protocol::ConfigurationClientbound>;

  void instrumentRegistry(csatle::network::TcpConnection& connection) {
    ConfigurationPacket packet(castle::protocol::ConfigurationClientbound::RegistryData);

    packet.addString("minecraft:instrument");

    packet.addVarInt(static_cast<std::int32_t>(INSTRUMENT_TYPES.size()));

    for (const auto& name : INSTRUMENT_TYPES) {

      packet.addString(name);

      packet.addByte(0x00);
    }
    castle::network::sendPacket(connection, packet);
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
