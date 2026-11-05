#pragma once

#include <cstdint>
namespace minecraft::protocol {
  enum class PlayServerbound : std::int32_t {
    Handshake = 0x00
  };
} // namespace minecraft::protocol
