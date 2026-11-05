#pragma once

#include <cstdint>
namespace minecraft::protocol {
  enum class StatusClientbound : std::int32_t {
    StatusResponse = 0x00,
    PongResponse = 0x01
  };
  enum class StatusServerbound : std::int32_t {
    StatusRequest = 0x00,
    PingRequest = 0x01
  };
} // namespace minecraft::protocol
