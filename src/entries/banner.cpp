#include "castle/entries/banner.hpp"
#include "castle/registry/banner_patterns.hpp"
#include <array>
#include <cstdint>

namespace castle::entries {

  void bannerTagsEntrie(ConfigurationPacket& packet) {
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
    packet.addString("minecraft:banner_pattern");

    // Number of tags
    packet.addVarInt(static_cast<std::int32_t>(tags.size()));

    for (const auto& [tagName, patternName] : tags) {
      packet.addString(tagName);

      // Each of these tags contains one pattern
      packet.addVarInt(1);

      packet.addVarInt(castle::registry::bannerPatternId(patternName));
    }
  }

} // namespace castle::entries
