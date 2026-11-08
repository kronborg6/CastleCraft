#pragma once

#include "castle/network/tcp_connection.hpp"
#include <array>
namespace castle::registry {

  static const std::array BIOME_TYPES = {
      "minecraft:plains",
  };

  void biomeRegistry(csatle::network::TcpConnection& connection);
} // namespace castle::registry
