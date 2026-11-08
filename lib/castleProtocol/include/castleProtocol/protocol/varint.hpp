
#pragma once
#include <cstdint>
#include <span>
#include <string>
#include <vector>

namespace castle::protocol {
  int32_t readVarInt(std::span<const std::uint8_t> data, std::size_t& offset);
  std::string readString(std::span<const std::uint8_t> data, std::size_t& offset);
  void writeVarInt(int32_t value, std::vector<uint8_t>& output);
} // namespace castle::protocol
