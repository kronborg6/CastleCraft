#pragma once
#include "castle/network/tcp_connection.hpp"
#include "castleProtocol/package/writePacket.hpp"
#include "castleProtocol/protocol/configuration.hpp"
#include <cstdint>
#include <vector>
namespace castle::entries {
  void damgeTypesEntrie(csatle::network::TcpConnection& connection);
  using ConfigurationPacket =
      castle::protocol::PacketWriter<castle::protocol::ConfigurationClientbound>;
  void explosionTypesEntrie(ConfigurationPacket& output);
  void fireTypesEntrie(ConfigurationPacket& output);
  void bypassShieldTypesEntrie(ConfigurationPacket& output);

  void fireExplosionTypes(csatle::network::TcpConnection& connection);
} // namespace castle::entries
