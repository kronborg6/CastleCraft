#pragma once

#include <cstdint>
#include <ostream>
#include <vector>

namespace castle::type {

  struct DataComponent {
      std::int32_t typeId;
      std::vector<std::uint8_t> data;
  };

  struct Slot {
      std::int32_t itemCount = 0;
      std::int32_t itemId = 0;

      std::vector<DataComponent> componentsToAdd;
      std::vector<std::int32_t> componentsToRemove;
  };
  inline std::ostream& operator<<(std::ostream& os, const Slot& slot) {
    os << "Slot { "
       << "itemCount: " << slot.itemCount << ", itemId: " << slot.itemId
       << ", componentsToAdd: " << slot.componentsToAdd.size()
       << ", componentsToRemove: " << slot.componentsToRemove.size() << " }";

    return os;
  }

} // namespace castle::type
