#pragma once
#include "../endian.hpp"

#include "castleProtocol/package/packet.hpp"
#include "castleProtocol/registry/damge_types.hpp"
#include "castleProtocol/types/postion.hpp"
#include "castleProtocol/types/slot.hpp"
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <span>
#include <stdexcept>
#include <string>
#include <tuple>

namespace castle::protocol {
  // i thinking about updateing all the function that return a value to getX inted of just x
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

      template <Integral T> [[nodiscard]] T Int() {
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

      [[nodiscard]] std::uint8_t byte() {
        if (_offset >= _data.size()) {
          throw std::out_of_range("PacketReader: Not enough bytes");
        }
        return _data[_offset++];
      }
      [[nodiscard]] std::span<const std::uint8_t> bytes(std::size_t length) {
        if (_offset > _data.size() || length > _data.size() - _offset) {
          throw std::runtime_error("Not enough bytes in packet");
        }

        auto result = _data.subspan(_offset, length);

        _offset += length;

        return result;
      }

      [[nodiscard]] std::int16_t getShort() {
        return endian::readBigEndian<std::int16_t>(_data, _offset);
      }

      [[nodiscard]] type::Postion getPosition() {
        const std::uint64_t packed = getInt<std::uint64_t>();

        auto signExtend = [](std::uint32_t value, int bits) -> std::int32_t {
          const std::uint32_t sign = 1U << (bits - 1);
          return static_cast<std::int32_t>((value ^ sign) - sign);
        };

        const std::int32_t x = signExtend(static_cast<std::uint32_t>(packed >> 38), 26);

        const std::int32_t y = signExtend(static_cast<std::uint32_t>(packed & 0xFFFULL), 12);

        const std::int32_t z =
            signExtend(static_cast<std::uint32_t>((packed >> 12) & 0x3FFFFFFULL), 26);

        return {x, y, z};
      }

      [[nodiscard]] float getFloat() {
        return endian::int32ToFloat(endian::readBigEndian<std::int32_t>(_data, _offset));
      }
      [[nodiscard]] double getDouble() {
        return endian::int64ToDouble(endian::readBigEndian<std::int64_t>(_data, _offset));
      }

      [[nodiscard]] type::Slot getSlot() {
        type::Slot slot;

        slot.itemCount = varInt();

        if (slot.itemCount < 0) {
          throw std::runtime_error("INvalid item count");
        }

        if (slot.itemCount == 0) {
          return slot;
        }

        slot.itemId = varInt();

        const auto addCount = varInt();
        const auto removeCount = varInt();

        if (addCount < 0 || removeCount < 0) {
          throw std::runtime_error("Invalid component count");
        }

        for (std::int32_t i = 0; i < addCount; ++i) {
          slot.componentsToAdd.push_back(readDataComponent());
        }

        for (std::int32_t i = 0; i < removeCount; ++i) {
          slot.componentsToRemove.push_back(varInt());
        }

        return slot;
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
      std::size_t position() const {
        return _offset;
      }

      std::vector<std::uint8_t> copyRange(std::size_t start, std::size_t end) const {
        if (start > end || end > _data.size()) {
          throw std::out_of_range("Invalid packet range");
        }

        return {_data.begin() + start, _data.begin() + end};
      }
      type::DataComponent readDataComponent() {
        type::DataComponent component;

        component.typeId = varInt();

        const auto start = position();

        auto type = static_cast<registry::DataComponentType>(component.typeId);

        switch (type) {
        case registry::DataComponentType::MaxStackSize:
        case registry::DataComponentType::MaxDamage:
        case registry::DataComponentType::Damage:
          varInt();
          break;
        case registry::DataComponentType::Unbreakable:
          // No data to read
          break;

        default:
          throw std::runtime_error("Unsupported component type: " +
                                   std::to_string(component.typeId));
        }

        component.data = copyRange(start, position());

        return component;
      }
  };
} // namespace castle::protocol
