#pragma once
#include "castle/network/tcp_connection.hpp"
#include <cstdint>
#include <vector>
namespace castle::entries {
  void damgeTypesEntrie(csatle::network::TcpConnection& connection);
  void explosionTypesEntrie(std::vector<std::uint8_t>& output);
  void fireTypesEntrie(std::vector<std::uint8_t>& output);
  void bypassShieldTypesEntrie(std::vector<std::uint8_t>& output);

  void fireExplosionTypes(csatle::network::TcpConnection& connection);
} // namespace castle::entries
