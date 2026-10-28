#pragma once

#include <cstdint>
#include <string>
#include <string_view>
#include <vector>
namespace castle::registry {
  struct DamageTypeData {
      std::string registryId;
      std::string messageId;
      float exhaustion;
      std::string scaling;
  };
  static const std::vector<DamageTypeData> DAMAGE_TYPES = {
      {"minecraft:cactus", "cactus", 0.1f, "when_caused_by_living_non_player"},
      {"minecraft:campfire", "inFire", 0.1f, "when_caused_by_living_non_player"},
      {"minecraft:cramming", "cramming", 0.0f, "when_caused_by_living_non_player"},
      {"minecraft:dragon_breath", "dragonBreath", 0.0f, "when_caused_by_living_non_player"},
      {"minecraft:drown", "drown", 0.0f, "when_caused_by_living_non_player"},
      {"minecraft:dry_out", "dryout", 0.1f, "when_caused_by_living_non_player"},
      {"minecraft:ender_pearl", "fall", 0.0f, "when_caused_by_living_non_player"},
      {"minecraft:fall", "fall", 0.0f, "when_caused_by_living_non_player"},
      {"minecraft:fly_into_wall", "flyIntoWall", 0.0f, "when_caused_by_living_non_player"},
      {"minecraft:freeze", "freeze", 0.0f, "when_caused_by_living_non_player"},
      {"minecraft:generic", "generic", 0.0f, "when_caused_by_living_non_player"},
      {"minecraft:generic_kill", "genericKill", 0.0f, "when_caused_by_living_non_player"},
      {"minecraft:hot_floor", "hotFloor", 0.1f, "when_caused_by_living_non_player"},
      {"minecraft:in_fire", "inFire", 0.1f, "when_caused_by_living_non_player"},
      {"minecraft:in_wall", "inWall", 0.0f, "when_caused_by_living_non_player"},
      {"minecraft:lava", "lava", 0.1f, "when_caused_by_living_non_player"},
      {"minecraft:lightning_bolt", "lightningBolt", 0.1f, "when_caused_by_living_non_player"},
      {"minecraft:magic", "magic", 0.0f, "when_caused_by_living_non_player"},
      {"minecraft:on_fire", "onFire", 0.0f, "when_caused_by_living_non_player"},
      {"minecraft:out_of_world", "outOfWorld", 0.0f, "when_caused_by_living_non_player"},
      {"minecraft:outside_border", "outsideBorder", 0.0f, "when_caused_by_living_non_player"},
      {"minecraft:spear", "spear", 0.1f, "when_caused_by_living_non_player"},
      {"minecraft:stalagmite", "stalagmite", 0.0f, "when_caused_by_living_non_player"},
      {"minecraft:starve", "starve", 0.0f, "when_caused_by_living_non_player"},
      {"minecraft:sweet_berry_bush", "sweetBerryBush", 0.1f, "when_caused_by_living_non_player"},
      {"minecraft:wither", "wither", 0.0f, "when_caused_by_living_non_player"}};

  std::uint16_t dameTypeId(std::string_view name);
} // namespace castle::registry
