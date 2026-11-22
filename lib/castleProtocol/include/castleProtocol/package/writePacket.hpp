#pragma once

#include "../endian.hpp"
#include "castleProtocol/package/packet.hpp"
#include "castleProtocol/protocol/varint.hpp"
#include "castleProtocol/types/postion.hpp"
#include <array>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <span>
#include <string>
#include <type_traits>
#include <vector>

namespace castle::protocol {
  using Uuid = std::array<std::uint8_t, 16>;
  template <Enum E> class PacketWriter {
    public:
      const E PacketId;
      explicit PacketWriter(E packetId) : PacketId(packetId) {
        writeVarInt(static_cast<std::int32_t>(packetId), _data);
      }

      void addByte(std::uint8_t byte) {
        _data.push_back(byte);
      }

      void addBoolean(bool boolean) {
        if (boolean) {
          _data.push_back(0x01);
        } else {
          _data.push_back(0x00);
        }
      }
      void addVarInt(std::int32_t value) {
        writeVarInt(value, _data);
      }

      template <std::integral T> void addInt(T value) {
        endian::writeBigEndian(value, _data);
      }
      void addString(std::string value) {
        writeVarInt(static_cast<std::int32_t>(value.size()), _data);

        _data.insert(_data.end(), value.begin(), value.end());
      }

      void addFloat(float value) {
        std::int32_t f = endian::floatToint32(value);
        endian::writeBigEndian<std::int32_t>(f, _data);
      }
      void addDouble(double value) {
        std::int64_t d = endian::doubleToint64(value);
        endian::writeBigEndian<std::int64_t>(d, _data);
      }

      void addBytes(std::span<const std::uint8_t> data) {
        _data.insert(_data.end(), data.begin(), data.end());
      }

      void addFloatArray(std::span<const float> values) {
        addVarInt(values.size());
        for (float v : values) {
          addFloat(v);
        }
      }

      void addUuid(Uuid uuid) {
        _data.insert(_data.end(), uuid.begin(), uuid.end());
      }

      void adUuidArray(std::span<const Uuid> uuids) {
        addVarInt(uuids.size());

        for (const Uuid& uuid : uuids) {
          addUuid(uuid);
        }
      }

      void addDoubleArray(std::span<const double> values) {
        addVarInt(values.size());
        for (double v : values) {
          addDouble(v);
        }
      }

      void addVarIntArray(std::span<const std::int32_t> values) {

        addVarInt(static_cast<std::int32_t>(values.size()));

        for (const auto& value : values) {
          addVarInt(value);
        }
      }

      void addPosition(type::Postion& postion) {
        const std::uint64_t packed =
            ((static_cast<std::uint64_t>(postion.x) & 0x3FFFFFFULL) << 38) |
            ((static_cast<std::uint64_t>(postion.z) & 0x3FFFFFFULL) << 12) |
            (static_cast<std::uint64_t>(postion.y) & 0xFFFULL);

        addInt<std::uint64_t>(packed);
      }
      void addPosition(std::int32_t x, std::int32_t y, std::int32_t z) {
        const std::uint64_t packed = ((static_cast<std::uint64_t>(x) & 0x3FFFFFFULL) << 38) |
                                     ((static_cast<std::uint64_t>(z) & 0x3FFFFFFULL) << 12) |
                                     (static_cast<std::uint64_t>(y) & 0xFFFULL);

        addInt<std::uint64_t>(packed);
      }

      [[nodiscard("why ask for it if you not gona look at it")]] std::size_t size() const {
        return _data.size();
      }

      [[nodiscard("why ask for the data if you not gona use it")]]
      std::span<const std::uint8_t> data() const {
        return _data;
      }

      [[nodiscard("Frame the packet before sending it")]]
      std::vector<std::uint8_t> frame() const {
        std::vector<std::uint8_t> result;
        result.reserve(_data.size() + 3);

        writeVarInt(static_cast<std::int32_t>(_data.size()), result);

        result.insert(result.end(), _data.begin(), _data.end());

        return result;
      }

    private:
      std::vector<std::uint8_t> _data;
  };
} // namespace castle::protocol
