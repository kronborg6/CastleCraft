// #include <castleProtocol/protocol/varint.hpp>
#include "castleProtocol/protocol/varint.hpp"
#include <stdexcept>

namespace castle::protocol {
  int32_t readVarInt(std::span<const std::uint8_t> data, std::size_t& offset) {
    std::uint32_t value = 0;

    for (int i = 0; i < 5; ++i) {
      if (offset >= data.size()) {
        throw std::runtime_error("Unexpected end of VarInt");
      }

      const std::uint8_t byte = data[offset++];

      // The fifth byte may only contain 4 value bits.
      if (i == 4 && (byte & 0xF0) != 0) {
        throw std::runtime_error("VarInt too large");
      }

      value |= static_cast<std::uint32_t>(byte & 0x7F) << (i * 7);

      if ((byte & 0x80) == 0) {
        return std::bit_cast<std::int32_t>(value);
      }
    }

    throw std::runtime_error("VarInt too large");
  }
  std::string readString(std::span<const std::uint8_t> data, std::size_t& offset) {
    const std::int32_t length = readVarInt(data, offset);

    if (length < 0 || static_cast<std::size_t>(length) > data.size() - offset) {
      throw std::runtime_error("Invalid string length");
    }

    std::string result(reinterpret_cast<const char*>(data.data() + offset),
                       static_cast<std::size_t>(length));

    offset += static_cast<std::size_t>(length);

    return result;
  }
} // namespace castle::protocol
