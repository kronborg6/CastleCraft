
#pragma once
#include <cstdint>
#include <span>
#include <string>

namespace castle::protocol {
  int32_t readVarInt(std::span<const std::uint8_t> data, std::size_t& offset);
  std::string readString(std::span<const std::uint8_t> data, std::size_t& offset);
} // namespace castle::protocol
