#pragma once

#include <cstdint>
namespace castle::item {
  struct Item {
      std::uint32_t itemId;
      std::uint8_t count = 1;
  };
} // namespace castle::item
