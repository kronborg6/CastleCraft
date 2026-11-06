#pragma once

#include <cstdint>
namespace castle::protocol {
  enum class HandshakeServerbound : std::int32_t {
    Handshake = 0x00
  };
} // namespace castle::protocol
