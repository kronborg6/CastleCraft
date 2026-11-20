#include "castle/item/inventory.hpp"

namespace castle::item {
  void Inventory::consumeItem(std::uint16_t slot) {
    auto& item = _items[slot];

    if (item.count <= 1) {
      this->remove(slot);
    }
    item.count--;
  }
  void Inventory::consumeHeldItem() {
    consumeItem(_selectedHotbarSlot);
  }
} // namespace castle::item
