#pragma once

#include <cstdint>
#include <span>
#include <string>
#include <vector>

namespace castle::protocol {
  std::int32_t readVarInt(std::span<const std::uint8_t> data, std::size_t& offset);
  void writeVarInt(int32_t value, std::vector<uint8_t>& output);
  std::string readString(std::span<const std::uint8_t> data, std::size_t& offset);
} // namespace castle::protocol
