#include "entries/damge_types.hpp"
#include "protocol/protocol.hpp"
#include "registry/damge_types.hpp"
#include "tcp.hpp"
#include <cstdint>
#include <vector>
namespace castle::entries {
  void damgeTypesEntrie(TcpConnection& connection) {

    std::vector<uint8_t> data;
    minecraft::protocol::writeString("minecraft:damage_type", data);
    minecraft::protocol::writeVarInt(static_cast<std::int32_t>(registry::DAMAGE_TYPES.size()),
                                     data);

    for (const auto& name : registry::DAMAGE_TYPES) {
      minecraft::protocol::writeString(name.registryId, data);
      data.push_back(0x00);
    }
    // Cold

    // Temperate

    minecraft::protocol::sendPacket(connection, 0x07, data);
  }

  void explosionTypesEntrie(std::vector<std::uint8_t>& output) {

    constexpr std::array explosionTypes = {"minecraft:fireworks",
                                           "minecraft:explosion",
                                           "minecraft:player_explosion",
                                           "minecraft:bad_respawn_point"};

    // Tag: minecraft:is_explosion
    minecraft::protocol::writeString("minecraft:is_explosion", output);

    // Number of entries
    minecraft::protocol::writeVarInt(explosionTypes.size(), output);

    for (const auto& name : explosionTypes) {
      minecraft::protocol::writeVarInt(castle::registry::dameTypeId(name), output);
    }
  }

  void fireTypesEntrie(std::vector<std::uint8_t>& output) {

    constexpr std::array fireTypes = {"minecraft:in_fire",
                                      "minecraft:campfire",
                                      "minecraft:on_fire",
                                      "minecraft:lava",
                                      "minecraft:hot_floor",
                                      "minecraft:sulfur_cube_hot",
                                      "minecraft:unattributed_fireball",
                                      "minecraft:fireball"};

    // minecraft:is_fire
    minecraft::protocol::writeString("minecraft:is_fire", output);

    // 0 entries (temporary)
    minecraft::protocol::writeVarInt(fireTypes.size(), output);

    for (const auto& name : fireTypes) {
      minecraft::protocol::writeVarInt(castle::registry::dameTypeId(name), output);
    }
  }
  void bypassShieldTypesEntrie(std::vector<std::uint8_t>& output) {

    constexpr std::array bypassesShield = {"minecraft:cactus",
                                           "minecraft:campfire",
                                           "minecraft:dry_out",
                                           "minecraft:falling_anvil",
                                           "minecraft:falling_stalactite",
                                           "minecraft:hot_floor",
                                           "minecraft:sulfur_cube_hot",
                                           "minecraft:in_fire",
                                           "minecraft:lava",
                                           "minecraft:lightning_bolt",
                                           "minecraft:sweet_berry_bush"};

    // Populate this from bypasses_armor.json
    constexpr std::array bypassesArmor = {"minecraft:on_fire",
                                          "minecraft:in_wall",
                                          "minecraft:cramming",
                                          "minecraft:drown",
                                          "minecraft:fly_into_wall",
                                          "minecraft:generic",
                                          "minecraft:wither",
                                          "minecraft:dragon_breath",
                                          "minecraft:starve",
                                          "minecraft:fall",
                                          "minecraft:ender_pearl",
                                          "minecraft:freeze",
                                          "minecraft:stalagmite",
                                          "minecraft:magic",
                                          "minecraft:indirect_magic",
                                          "minecraft:out_of_world",
                                          "minecraft:generic_kill",
                                          "minecraft:sonic_boom",
                                          "minecraft:outside_border"};

    std::vector<std::int32_t> shieldIds;

    auto appendIds = [&](const auto& names) {
      for (const auto& name : names) {
        auto id = castle::registry::dameTypeId(name);

        if (std::ranges::find(shieldIds, id) == shieldIds.end()) {
          shieldIds.push_back(id);
        }
      }
    };

    appendIds(bypassesArmor);
    appendIds(bypassesShield);

    minecraft::protocol::writeString("minecraft:bypasses_shield", output);

    minecraft::protocol::writeVarInt(static_cast<std::int32_t>(shieldIds.size()), output);

    for (auto id : shieldIds) {
      minecraft::protocol::writeVarInt(id, output);
    }
  }

  void fireExplosionTypes(TcpConnection& connection) {
    std::vector<std::uint8_t> output;
    minecraft::protocol::writeVarInt(2, output);

    minecraft::protocol::writeString("minecraft:damage_type", output);

    // Number of tags
    minecraft::protocol::writeVarInt(3, output);

    explosionTypesEntrie(output);
    fireTypesEntrie(output);
    bypassShieldTypesEntrie(output);

    minecraft::protocol::writeString("minecraft:timeline", output);

    // Number of tags
    minecraft::protocol::writeVarInt(2, output);

    // Tag 0: minecraft:universal
    minecraft::protocol::writeString("minecraft:universal", output);
    minecraft::protocol::writeVarInt(1, output);
    minecraft::protocol::writeVarInt(3, output);

    // Tag 1: minecraft:in_overworld
    minecraft::protocol::writeString("minecraft:in_overworld", output);
    minecraft::protocol::writeVarInt(4, output);

    for (std::int32_t id = 0; id < 4; ++id) {
      minecraft::protocol::writeVarInt(id, output);
    }

    // Send Update Tags
    minecraft::protocol::sendPacket(connection, 0x0E, output);
  }
} // namespace castle::entries
