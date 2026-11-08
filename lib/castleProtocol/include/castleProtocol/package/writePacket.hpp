#pragma once

#include "../endian.hpp"
#include "castleProtocol/package/packet.hpp"
#include "castleProtocol/protocol/varint.hpp"
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <string>
#include <type_traits>
#include <vector>

namespace castle::protocol {
  template <Enum E> class PacketWriter {
    public:
      const E PacketId;
      explicit PacketWriter(E packetId) : PacketId(packetId) {
        writeVarInt(static_cast<std::int32_t>(packetId), _data);
      }

      void addByte(std::uint8_t byte) {
        _data.push_back(byte);
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
