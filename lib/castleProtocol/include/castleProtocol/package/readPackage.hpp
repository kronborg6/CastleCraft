#pragma once
#include "../endian.hpp"

#include "castleProtocol/protocol/varint.hpp"
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <span>
#include <type_traits>
#include <utility>
#include <vector>

template <typename T>
concept Integral = std::is_integral_v<T>;

namespace castle::protocol {
  template <typename T>
  concept Enum = std::is_enum_v<T>;
  template <Enum E> class Package {
    public:
      Package(std::vector<std::uint8_t> data) : _data(std::move(data)) {
        _packageId = static_cast<E>(readVarInt(data, _offset));
      };
      Package(Package&&) = default;
      Package(const Package&) = default;
      Package& operator=(Package&&) = default;
      Package& operator=(const Package&) = default;
      ~Package() = default;

      E getPackgetId() const {
        return _packageId;
      }

      template <Integral T> [[nodiscard]] T getInt() {
        return endian::readBigEndian<T>(_data, _offset);
      }

    private:
      std::span<const std::uint8_t> _data;
      std::size_t _offset = 0;
      E _packageId{};
  };
} // namespace castle::protocol
