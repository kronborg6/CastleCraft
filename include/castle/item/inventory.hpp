#pragma once

#include "item.hpp"
#include <cstdint>
#include <map>
#include <stdexcept>
#include <utility>

namespace castle::item {
  class Inventory {
    public:
      static constexpr std::uint8_t HOTBAR_SIZE = 9;
      static constexpr std::uint8_t INVENTORY_SIZE = 36;

      void insert(std::uint16_t slot, Item item) {
        checkRange(INVENTORY_SIZE, slot);

        _items.insert_or_assign(slot, std::move(item));
      }
      void remove(std::uint16_t slot) {
        checkRange(INVENTORY_SIZE, slot);
        _items.erase(slot);
      }

      [[nodiscard]] Item* getItem(std::uint16_t slot) {
        checkRange(INVENTORY_SIZE, slot);
        auto it = _items.find(slot);

        return it != _items.end() ? &it->second : nullptr;
      }

      [[nodiscard]] Item* getHeldItem() {
        return getItem(_selectedHotbarSlot);
      }

      void updateSelectedHotbarSlot(std::uint8_t slot) {
        checkRange(HOTBAR_SIZE, slot);

        _selectedHotbarSlot = slot;
      }

    private:
      std::map<std::uint16_t, Item> _items;
      std::uint8_t _selectedHotbarSlot = 0;

      void checkRange(std::uint8_t max, std::uint16_t slot) const {
        if (slot >= max) {
          throw std::out_of_range("Slot must be between 0 and " + std::to_string(max - 1));
        }
      }
  };
} // namespace castle::item
