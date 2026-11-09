#pragma once
#include <cstdint>
#include <span>
#include <stdexcept>
#include <vector>

namespace castle::endian {
  template <std::integral T> void writeBigEndian(T value, std::vector<std::uint8_t>& data) {
    for (int shift = static_cast<int>(sizeof(T) * 8) - 8; shift >= 0; shift -= 8) {
      data.push_back(static_cast<std::uint8_t>((value >> shift) & 0xFF));
    }
  }

  template <std::unsigned_integral T>
  void writeLittleEndian(T value, std::vector<std::uint8_t>& data) {
    for (int shift = 0; shift < static_cast<int>(sizeof(T) * 8); shift += 8) {
      data.push_back(static_cast<std::uint8_t>((value >> shift) & 0xFF));
    }
  }

  template <std::integral U>
  [[nodiscard]] U readBigEndian(std::span<const std::uint8_t> data, std::size_t& offset) {
    if (offset > data.size() || data.size() - offset < sizeof(U)) {
      throw std::runtime_error("readBigEndian: unexpected end of data");
    }
    U value = 0;
    for (std::size_t i = 0; i < sizeof(U); ++i) {
      value = static_cast<U>((value << 8) | data[offset + i]);
    }
    offset += sizeof(U);
    return value;
  }

  uint32_t floatToUint32(float from);
  int32_t floatToint32(float from);
  uint64_t doubleToUint64(double from);
  int64_t doubleToint64(double value);
  int64_t longToInt64(long value);

  long int64ToLong(int64_t from);

  float int32ToFloat(int32_t from);
  double int64ToDouble(int64_t from);
  float uint32ToFloat(uint32_t from);
  double uint64ToDouble(uint64_t from);
} // namespace castle::endian
