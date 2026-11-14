#include "castle/entries/mobs.hpp"
#include "castle/network/packet_send.hpp"
#include "castle/network/tcp_connection.hpp"
#include "castleProtocol/package/writePacket.hpp"
#include "castleProtocol/protocol/configuration.hpp"
#include <array>
#include <cstdint>

namespace castle::entries {
  using ConfigurationPacket =
      castle::protocol::PacketWriter<castle::protocol::ConfigurationClientbound>;

  void cowEntrie(castle::network::TcpConnection& connection) {
    ConfigurationPacket packet(castle::protocol::ConfigurationClientbound::RegistryData);
    packet.addString("minecraft:cow_variant");

    packet.addVarInt(3);

    for (const auto& variant : {"minecraft:cold", "minecraft:temperate", "minecraft:warm"}) {

      packet.addString(variant);

      packet.addByte(0x00);
    }

    castle::network::sendPacket(connection, packet);
  }

  void cowSoundEntrie(castle::network::TcpConnection& connection) {
    ConfigurationPacket packet(castle::protocol::ConfigurationClientbound::RegistryData);

    packet.addString("minecraft:cow_sound_variant");

    packet.addVarInt(2);

    for (const auto& variant : {"minecraft:classic", "minecraft:moody"}) {

      packet.addString(variant);

      packet.addByte(0x00);
    }

    castle::network::sendPacket(connection, packet);
  }

  void catEntrie(castle::network::TcpConnection& connection) {
    ConfigurationPacket packet(castle::protocol::ConfigurationClientbound::RegistryData);
    packet.addString("minecraft:cat_variant");
    constexpr std::array variants = {"minecraft:all_black",
                                     "minecraft:british_shorthair",
                                     "minecraft:calico",
                                     "minecraft:jellie",
                                     "minecraft:persian",
                                     "minecraft:ragdoll",
                                     "minecraft:red",
                                     "minecraft:siamese",
                                     "minecraft:tabby",
                                     "minecraft:black",
                                     "minecraft:white"};

    packet.addVarInt(static_cast<std::int32_t>(variants.size()));
    for (const auto& variant : variants) {
      packet.addString(variant);
      packet.addByte(0x00);
    }
    castle::network::sendPacket(connection, packet);
  }

  void catSoundEntrie(castle::network::TcpConnection& connection) {

    ConfigurationPacket packet(castle::protocol::ConfigurationClientbound::RegistryData);

    packet.addString("minecraft:cat_sound_variant");

    packet.addVarInt(2);

    for (const auto& variant : {"minecraft:classic", "minecraft:royal"}) {

      packet.addString(variant);

      packet.addByte(0x00);
    }

    castle::network::sendPacket(connection, packet);
  }

  void chickenEntrie(castle::network::TcpConnection& connection) {

    ConfigurationPacket packet(castle::protocol::ConfigurationClientbound::RegistryData);

    packet.addString("minecraft:chicken_variant");

    packet.addVarInt(3);

    for (const auto& variant : {"minecraft:cold", "minecraft:temperate", "minecraft:warm"}) {

      packet.addString(variant);

      packet.addByte(0x00);
    }

    castle::network::sendPacket(connection, packet);
  }

  void chickenSoundEntrie(castle::network::TcpConnection& connection) {

    ConfigurationPacket packet(castle::protocol::ConfigurationClientbound::RegistryData);

    packet.addString("minecraft:chicken_sound_variant");

    packet.addVarInt(2);

    for (const auto& variant : {"minecraft:classic", "minecraft:picky"}) {

      packet.addString(variant);

      packet.addByte(0x00);
    }

    castle::network::sendPacket(connection, packet);
  }

  void frogEntrie(castle::network::TcpConnection& connection) {

    ConfigurationPacket packet(castle::protocol::ConfigurationClientbound::RegistryData);

    packet.addString("minecraft:frog_variant");

    packet.addVarInt(3);

    for (const auto& variant : {"minecraft:cold", "minecraft:temperate", "minecraft:warm"}) {

      packet.addString(variant);

      packet.addByte(0x00);
    }

    castle::network::sendPacket(connection, packet);
  }

  void paintingEntrie(castle::network::TcpConnection& connection) {

    ConfigurationPacket packet(castle::protocol::ConfigurationClientbound::RegistryData);

    packet.addString("minecraft:painting_variant");

    constexpr std::array variants = {"minecraft:alban",        "minecraft:aztec",
                                     "minecraft:aztec2",       "minecraft:backyard",
                                     "minecraft:baroque",      "minecraft:bomb",
                                     "minecraft:bouquet",      "minecraft:burning_skull",
                                     "minecraft:bust",         "minecraft:cavebird",
                                     "minecraft:changing",     "minecraft:cotan",
                                     "minecraft:courbet",      "minecraft:creebet",
                                     "minecraft:dennis",       "minecraft:donkey_kong",
                                     "minecraft:earth",        "minecraft:endboss",
                                     "minecraft:fern",         "minecraft:fighters",
                                     "minecraft:finding",      "minecraft:fire",
                                     "minecraft:graham",       "minecraft:humble",
                                     "minecraft:kebab",        "minecraft:lowmist",
                                     "minecraft:match",        "minecraft:meditative",
                                     "minecraft:orb",          "minecraft:owlemons",
                                     "minecraft:passage",      "minecraft:pigscene",
                                     "minecraft:plant",        "minecraft:pointer",
                                     "minecraft:pond",         "minecraft:pool",
                                     "minecraft:prairie_ride", "minecraft:sea",
                                     "minecraft:skeleton",     "minecraft:skull_and_roses",
                                     "minecraft:stage",        "minecraft:sunflowers",
                                     "minecraft:sunset",       "minecraft:tides",
                                     "minecraft:unpacked",     "minecraft:void",
                                     "minecraft:wanderer",     "minecraft:wasteland",
                                     "minecraft:water",        "minecraft:wind",
                                     "minecraft:wither"};

    packet.addVarInt(static_cast<std::int32_t>(variants.size()));

    for (const auto& variant : variants) {

      packet.addString(variant);

      packet.addByte(0x00);
    }

    castle::network::sendPacket(connection, packet);
  }

  void pigEntrie(castle::network::TcpConnection& connection) {

    ConfigurationPacket packet(castle::protocol::ConfigurationClientbound::RegistryData);

    packet.addString("minecraft:pig_variant");

    packet.addVarInt(3);

    for (const auto& variant : {"minecraft:cold", "minecraft:temperate", "minecraft:warm"}) {

      packet.addString(variant);

      packet.addByte(0x00);
    }

    castle::network::sendPacket(connection, packet);
  }

  void pigSoundEntrie(castle::network::TcpConnection& connection) {

    ConfigurationPacket packet(castle::protocol::ConfigurationClientbound::RegistryData);

    packet.addString("minecraft:pig_sound_variant");

    packet.addVarInt(3);

    for (const auto& variant : {"minecraft:big", "minecraft:classic", "minecraft:mini"}) {

      packet.addString(variant);

      packet.addByte(0x00);
    }

    castle::network::sendPacket(connection, packet);
  }

  void wolfEntrie(castle::network::TcpConnection& connection) {

    ConfigurationPacket packet(castle::protocol::ConfigurationClientbound::RegistryData);

    packet.addString("minecraft:wolf_variant");

    constexpr std::array variants = {"minecraft:ashen",
                                     "minecraft:black",
                                     "minecraft:chestnut",
                                     "minecraft:pale",
                                     "minecraft:rusty",
                                     "minecraft:snowy",
                                     "minecraft:spotted",
                                     "minecraft:striped",
                                     "minecraft:woods"};

    packet.addVarInt(static_cast<std::int32_t>(variants.size()));

    for (const auto& variant : variants) {

      packet.addString(variant);

      packet.addByte(0x00);
    }

    castle::network::sendPacket(connection, packet);
  }

  void wolfSoundEntrie(castle::network::TcpConnection& connection) {

    ConfigurationPacket packet(castle::protocol::ConfigurationClientbound::RegistryData);

    packet.addString("minecraft:wolf_sound_variant");

    constexpr std::array variants = {"minecraft:big",
                                     "minecraft:classic",
                                     "minecraft:cute",
                                     "minecraft:puglin",
                                     "minecraft:angry",
                                     "minecraft:grumpy",
                                     "minecraft:sad"};

    packet.addVarInt(static_cast<std::int32_t>(variants.size()));

    for (const auto& variant : variants) {

      packet.addString(variant);

      packet.addByte(0x00);
    }

    castle::network::sendPacket(connection, packet);
  }

  void zombieEntrie(castle::network::TcpConnection& connection) {

    ConfigurationPacket packet(castle::protocol::ConfigurationClientbound::RegistryData);

    packet.addString("minecraft:zombie_nautilus_variant");

    constexpr std::array variants = {"minecraft:temperate", "minecraft:warm"};

    packet.addVarInt(static_cast<std::int32_t>(variants.size()));

    for (const auto& variant : variants) {

      packet.addString(variant);

      packet.addByte(0x00);
    }

    castle::network::sendPacket(connection, packet);
  }
} // namespace castle::entries
