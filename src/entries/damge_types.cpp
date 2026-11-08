#include "castle/entries/damge_types.hpp"
#include "castle/network/packet_send.hpp"
#include "castle/network/tcp_connection.hpp"
#include "castle/registry/damge_types.hpp"
#include "castleProtocol/protocol/configuration.hpp"
#include <cstdint>
#include <vector>
namespace castle::entries {
  using ConfigurationPacket =
      castle::protocol::PacketWriter<castle::protocol::ConfigurationClientbound>;

  void damgeTypesEntrie(csatle::network::TcpConnection& connection) {

    ConfigurationPacket packet(castle::protocol::ConfigurationClientbound::RegistryData);

    packet.addString("minecraft:damage_type");

    packet.addVarInt(static_cast<std::int32_t>(registry::DAMAGE_TYPES.size()));

    for (const auto& name : registry::DAMAGE_TYPES) {

      packet.addString(name.registryId);

      packet.addByte(0x00);
    }

    castle::network::sendPacket(connection, packet);
  }

  void explosionTypesEntrie(ConfigurationPacket& packet) {

    constexpr std::array explosionTypes = {"minecraft:fireworks",
                                           "minecraft:explosion",
                                           "minecraft:player_explosion",
                                           "minecraft:bad_respawn_point"};

    // Tag: minecraft:is_explosion
    packet.addString("minecraft:is_explosion");

    // Number of entries
    packet.addVarInt(static_cast<std::int32_t>(explosionTypes.size()));

    for (const auto& name : explosionTypes) {
      packet.addVarInt(castle::registry::dameTypeId(name));
    }
  }

  void fireTypesEntrie(ConfigurationPacket& packet) {

    constexpr std::array fireTypes = {"minecraft:in_fire",
                                      "minecraft:campfire",
                                      "minecraft:on_fire",
                                      "minecraft:lava",
                                      "minecraft:hot_floor",
                                      "minecraft:sulfur_cube_hot",
                                      "minecraft:unattributed_fireball",
                                      "minecraft:fireball"};

    // minecraft:is_fire
    packet.addString("minecraft:is_fire");

    // 0 entries (temporary)
    packet.addVarInt(static_cast<std::int32_t>(fireTypes.size()));

    for (const auto& name : fireTypes) {
      packet.addVarInt(castle::registry::dameTypeId(name));
    }
  }
  void bypassShieldTypesEntrie(ConfigurationPacket& packet) {

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

    packet.addString("minecraft:bypasses_shield");

    packet.addVarInt(static_cast<std::int32_t>(shieldIds.size()));

    for (auto id : shieldIds) {
      packet.addVarInt(id);
    }
  }

} // namespace castle::entries
