#pragma once

#include "castle/network/tcp_connection.hpp"
#include <cstdint>
#include <string>
#include <vector>
namespace castle::registry {
  static const std::vector<std::string> INSTRUMENT_TYPES = {
      "minecraft:ponder_goat_horn",
      "minecraft:sing_goat_horn",
      "minecraft:seek_goat_horn",
      "minecraft:feel_goat_horn",
      "minecraft:admire_goat_horn",
      "minecraft:call_goat_horn",
      "minecraft:yearn_goat_horn",
      "minecraft:dream_goat_horn",
  };

  void instrumentRegistry(csatle::network::TcpConnection& connection);
  std::uint16_t instrumentTypeId(std::string_view name);
} // namespace castle::registry
