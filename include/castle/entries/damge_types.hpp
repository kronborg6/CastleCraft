#pragma once
#include "castle/network/tcp_connection.hpp"
#include "castleProtocol/package/writePacket.hpp"
#include "castleProtocol/protocol/configuration.hpp"
#include <cstdint>
#include <vector>
namespace castle::entries {
  void damgeTypesEntrie(castle::network::TcpConnection& connection);
  using ConfigurationPacket =
      castle::protocol::PacketWriter<castle::protocol::ConfigurationClientbound>;
  void explosionTypesEntrie(ConfigurationPacket& output);
  void fireTypesEntrie(ConfigurationPacket& output);
  void bypassShieldTypesEntrie(ConfigurationPacket& output);

  void fireExplosionTypes(castle::network::TcpConnection& connection);
} // namespace castle::entries
