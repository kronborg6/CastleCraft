#pragma once
#include "../endian.hpp"

#include "castleProtocol/protocol/varint.hpp"
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <span>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

template <typename T>
concept Integral = std::is_integral_v<T>;

namespace castle::protocol {
  template <typename T>
  concept Enum = std::is_enum_v<T>;
  template <Enum E> class PacketReader {
      // fields order does matter
      // is it good to need a order
      // no
    private:
      std::size_t _offset = 0;
      std::span<const std::uint8_t> _data;

    public:
      const E PacketId;

    public:
      explicit PacketReader(std::span<const std::uint8_t> data)
          : _data(data), PacketId(static_cast<E>(readVarInt(data, _offset))) {};

      template <Integral T> [[nodiscard]] T getInt() {
        return endian::readBigEndian<T>(_data, _offset);
      }

      [[nodiscard]] std::int32_t varInt() {
        return this->readVarInt(_data, _offset);
      }
      [[nodiscard]] std::string string() {
        const std::int32_t length = varInt();

        if (length < 0 || static_cast<std::size_t>(length) > _data.size() - _offset) {
          throw std::runtime_error("Invalid string length");
        }

        const auto start = _data.begin() + static_cast<std::ptrdiff_t>(_offset);
        const auto end = start + static_cast<std::ptrdiff_t>(length);

        std::string result(start, end);

        _offset += static_cast<std::size_t>(length);

        return result;
      }
      [[nodiscard]] std::span<const std::uint8_t> bytes(std::size_t length) {
        if (_offset > _data.size() || length > _data.size() - _offset) {
          throw std::runtime_error("Not enough bytes in packet");
        }

        auto result = _data.subspan(_offset, length);

        _offset += length;

        return result;
      }

    private:
      static std::int32_t readVarInt(std::span<const std::uint8_t> data, std::size_t& offset) {
        // std::size_t offset = 0;
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
  };
} // namespace castle::protocol
