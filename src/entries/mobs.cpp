#include "entries/mobs.hpp"
#include "protocol/protocol.hpp"
#include "tcp.hpp"
#include <cstdint>
#include <vector>

namespace castle::entries {
  void cowEntrie(TcpConnection& connection) {

    std::vector<uint8_t> data;
    minecraft::protocol::writeString("minecraft:cow_variant", data);
    minecraft::protocol::writeVarInt(3, data);
    // Cold
    minecraft::protocol::writeString("minecraft:cold", data);
    data.push_back(0x00);

    // Temperate
    minecraft::protocol::writeString("minecraft:temperate", data);
    data.push_back(0x00);

    // Warm
    minecraft::protocol::writeString("minecraft:warm", data);
    data.push_back(0x00);

    minecraft::protocol::sendPacket(connection, 0x07, data);
  }
  void cowSoundEntrie(TcpConnection& connection) {
    std::vector<std::uint8_t> data;
    minecraft::protocol::writeString("minecraft:cow_sound_variant", data);
    minecraft::protocol::writeVarInt(2, data);

    minecraft::protocol::writeString("minecraft:classic", data);
    data.push_back(0x00);
    minecraft::protocol::writeString("minecraft:moody", data);
    data.push_back(0x00);

    minecraft::protocol::sendPacket(connection, 0x07, data);
  }

  void catEntrie(TcpConnection& connection) {
    std::vector<std::uint8_t> data;

    minecraft::protocol::writeString("minecraft:cat_variant", data);
    minecraft::protocol::writeVarInt(11, data);

    for (const auto& variant : {"minecraft:all_black",
                                "minecraft:british_shorthair",
                                "minecraft:calico",
                                "minecraft:jellie",
                                "minecraft:persian",
                                "minecraft:ragdoll",
                                "minecraft:red",
                                "minecraft:siamese",
                                "minecraft:tabby",
                                "minecraft:black",
                                "minecraft:white"}) {
      minecraft::protocol::writeString(variant, data);
      data.push_back(0x00);
    }

    minecraft::protocol::sendPacket(connection, 0x07, data);
  }
  void catSoundEntrie(TcpConnection& connection) {

    std::vector<std::uint8_t> data;
    minecraft::protocol::writeString("minecraft:cat_sound_variant", data);
    minecraft::protocol::writeVarInt(2, data);

    minecraft::protocol::writeString("minecraft:classic", data);
    data.push_back(0x00);
    minecraft::protocol::writeString("minecraft:royal", data);
    data.push_back(0x00);

    minecraft::protocol::sendPacket(connection, 0x07, data);
  }
  void chickenEntrie(TcpConnection& connection) {
    std::vector<uint8_t> data;
    minecraft::protocol::writeString("minecraft:chicken_variant", data);
    minecraft::protocol::writeVarInt(3, data);
    // Cold
    minecraft::protocol::writeString("minecraft:cold", data);
    data.push_back(0x00);

    // Temperate
    minecraft::protocol::writeString("minecraft:temperate", data);
    data.push_back(0x00);

    // Warm
    minecraft::protocol::writeString("minecraft:warm", data);
    data.push_back(0x00);

    minecraft::protocol::sendPacket(connection, 0x07, data);
  }
  void chickenSoundEntrie(TcpConnection& connection) {
    std::vector<std::uint8_t> data;
    minecraft::protocol::writeString("minecraft:chicken_sound_variant", data);
    minecraft::protocol::writeVarInt(2, data);

    minecraft::protocol::writeString("minecraft:classic", data);
    data.push_back(0x00);
    minecraft::protocol::writeString("minecraft:picky", data);
    data.push_back(0x00);

    minecraft::protocol::sendPacket(connection, 0x07, data);
  }
  void frogEntrie(TcpConnection& connection) {
    std::vector<uint8_t> data;
    minecraft::protocol::writeString("minecraft:frog_variant", data);
    minecraft::protocol::writeVarInt(3, data);
    // Cold
    minecraft::protocol::writeString("minecraft:cold", data);
    data.push_back(0x00);

    // Temperate
    minecraft::protocol::writeString("minecraft:temperate", data);
    data.push_back(0x00);

    // Warm
    minecraft::protocol::writeString("minecraft:warm", data);
    data.push_back(0x00);

    minecraft::protocol::sendPacket(connection, 0x07, data);
  }
  void paintingEntrie(TcpConnection& connection) {
    std::vector<std::uint8_t> data;

    minecraft::protocol::writeString("minecraft:painting_variant", data);

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

    minecraft::protocol::writeVarInt(variants.size(), data);

    for (const auto& variant : variants) {
      minecraft::protocol::writeString(variant, data);
      data.push_back(0x00);
    }

    minecraft::protocol::sendPacket(connection, 0x07, data);
  }
  void pigEntrie(TcpConnection& connection) {
    std::vector<uint8_t> data;
    minecraft::protocol::writeString("minecraft:pig_variant", data);
    minecraft::protocol::writeVarInt(3, data);
    // Cold
    minecraft::protocol::writeString("minecraft:cold", data);
    data.push_back(0x00);

    // Temperate
    minecraft::protocol::writeString("minecraft:temperate", data);
    data.push_back(0x00);

    // Warm
    minecraft::protocol::writeString("minecraft:warm", data);
    data.push_back(0x00);

    minecraft::protocol::sendPacket(connection, 0x07, data);
  }
  void pigSoundEntrie(TcpConnection& connection) {
    std::vector<uint8_t> data;
    minecraft::protocol::writeString("minecraft:pig_sound_variant", data);
    minecraft::protocol::writeVarInt(3, data);
    // Cold
    minecraft::protocol::writeString("minecraft:big", data);
    data.push_back(0x00);

    // Temperate
    minecraft::protocol::writeString("minecraft:classic", data);
    data.push_back(0x00);

    // Warm
    minecraft::protocol::writeString("minecraft:mini", data);
    data.push_back(0x00);

    minecraft::protocol::sendPacket(connection, 0x07, data);
  }
  void wolfEntrie(TcpConnection& connection) {
    std::vector<std::uint8_t> data;

    minecraft::protocol::writeString("minecraft:wolf_variant", data);
    minecraft::protocol::writeVarInt(9, data);

    for (const auto& variant : {"minecraft:ashen",
                                "minecraft:black",
                                "minecraft:chestnut",
                                "minecraft:pale",
                                "minecraft:rusty",
                                "minecraft:snowy",
                                "minecraft:spotted",
                                "minecraft:striped",
                                "minecraft:woods"}) {
      minecraft::protocol::writeString(variant, data);
      data.push_back(0x00);
    }

    minecraft::protocol::sendPacket(connection, 0x07, data);
  }
  void wolfSoundEntrie(TcpConnection& connection) {
    std::vector<std::uint8_t> data;

    minecraft::protocol::writeString("minecraft:wolf_sound_variant", data);
    minecraft::protocol::writeVarInt(7, data);

    for (const auto& variant : {"minecraft:big",
                                "minecraft:classic",
                                "minecraft:cute",
                                "minecraft:puglin",
                                "minecraft:angry",
                                "minecraft:grumpy",
                                "minecraft:sad"}) {
      minecraft::protocol::writeString(variant, data);
      data.push_back(0x00);
    }

    minecraft::protocol::sendPacket(connection, 0x07, data);
  }
  void zombieEntrie(TcpConnection& connection) {
    std::vector<std::uint8_t> data;

    minecraft::protocol::writeString("minecraft:zombie_nautilus_variant", data);
    minecraft::protocol::writeVarInt(2, data);

    for (const auto& variant : {
             "minecraft:temperate",
             "minecraft:warm",
         }) {
      minecraft::protocol::writeString(variant, data);
      data.push_back(0x00);
    }

    minecraft::protocol::sendPacket(connection, 0x07, data);
  }

} // namespace castle::entries
