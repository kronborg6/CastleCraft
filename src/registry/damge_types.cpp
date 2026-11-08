#include "castle/registry/damge_types.hpp"
#include <algorithm>
#include <cstdint>
#include <iterator>
#include <stdexcept>
#include <string>

namespace castle::registry {
  std::uint16_t dameTypeId(std::string_view name) {
    auto it = std::find_if(DAMAGE_TYPES.begin(), DAMAGE_TYPES.end(), [&](const DamageTypeData& d) {
      return d.registryId == name;
    });

    if (it == DAMAGE_TYPES.end()) {
      throw std::runtime_error("unknown damge type: " + std::string(name));
    }

    return static_cast<std::uint16_t>(std::distance(DAMAGE_TYPES.begin(), it));
  }
} // namespace castle::registry
