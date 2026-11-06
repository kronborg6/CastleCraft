#pragma once

#include "tcp.hpp"
#include <array>
namespace castle::registry {

  static const std::array BIOME_TYPES = {
      "minecraft:plains",
  };

  void biomeRegistry(TcpConnection& connection);
} // namespace castle::registry
