#pragma once
#include "tcp.hpp"
#include <cstdint>
#include <vector>
namespace castle::entries {
  void damgeTypesEntrie(TcpConnection& connection);
  void explosionTypesEntrie(std::vector<std::uint8_t>& output);
  void fireTypesEntrie(std::vector<std::uint8_t>& output);

  void fireExplosionTypes(TcpConnection& connection);
} // namespace castle::entries
