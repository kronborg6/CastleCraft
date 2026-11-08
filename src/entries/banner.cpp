#include "entries/banner.hpp"
#include "protocol/protocol.hpp"
#include "registry/banner_patterns.hpp"
#include <array>
#include <cstdint>
#include <vector>

namespace castle::entries {

  void bannerTagsEntrie(std::vector<std::uint8_t>& output) {
    constexpr std::array tags = {
        std::pair{"minecraft:pattern_item/bordure_indented", "minecraft:curly_border"},
        std::pair{"minecraft:pattern_item/creeper", "minecraft:creeper"},
        std::pair{"minecraft:pattern_item/field_masoned", "minecraft:bricks"},
        std::pair{"minecraft:pattern_item/flow", "minecraft:flow"},
        std::pair{"minecraft:pattern_item/flower", "minecraft:flower"},
        std::pair{"minecraft:pattern_item/globe", "minecraft:globe"},
        std::pair{"minecraft:pattern_item/guster", "minecraft:guster"},
        std::pair{"minecraft:pattern_item/mojang", "minecraft:mojang"},
        std::pair{"minecraft:pattern_item/piglin", "minecraft:piglin"},
        std::pair{"minecraft:pattern_item/skull", "minecraft:skull"}};

    // Registry identifier
    minecraft::protocol::writeString("minecraft:banner_pattern", output);

    // Number of tags
    minecraft::protocol::writeVarInt(static_cast<std::int32_t>(tags.size()), output);

    for (const auto& [tagName, patternName] : tags) {
      minecraft::protocol::writeString(tagName, output);

      // Each of these tags contains one pattern
      minecraft::protocol::writeVarInt(1, output);

      minecraft::protocol::writeVarInt(castle::registry::bannerPatternId(patternName), output);
    }
  }

} // namespace castle::entries
