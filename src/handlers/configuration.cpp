#include "handlers/configuration.hpp"
#include "castle/nbt.hpp"
#include "castle/network/packet_receiver.hpp"
#include "castle/network/tcp_connection.hpp"
#include "castle/reader.hpp"
#include "castle/writer.hpp"
#include "client.hpp"
#include "entries/damge_types.hpp"
#include "entries/entries.hpp"
#include "entries/jukebox.hpp"
#include "entries/mobs.hpp"
#include "handlers/play.hpp"
#include "io/nbt_loader.hpp"
// #include "protocol/configuration.hpp"
#include "protocol/protocol.hpp"
#include "registry/banner_patterns.hpp"
#include "registry/biome.hpp"
#include "registry/instrument.hpp"
#include "registry/pottery.hpp"
#include <array>
#include <castleProtocol/package/readPackage.hpp>
#include <castleProtocol/protocol/configuration.hpp>
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

using namespace minecraft::protocol;
namespace {
  void sendEnty(csatle::network::TcpConnection& connection) {
    std::vector<uint8_t> data;
    std::vector<uint8_t> rawNbt;

    castle::io::appendsNbtFile("./generated/dimensions/overworld.nbt", rawNbt);

    minecraft::protocol::writeString("minecraft:dimension_type", data);

    // zero entries
    minecraft::protocol::writeVarInt(1, data);

    minecraft::protocol::writeString("minecraft:overworld", data);

    data.push_back(0x01);

    data.insert(data.end(), rawNbt.begin(), rawNbt.end());

    minecraft::protocol::sendPacket(
        connection, castle::protocol::ConfigurationClientbound::RegistryData, data);
  }
  void sendWorldClockRegistry(csatle::network::TcpConnection& connection) {
    std::vector<uint8_t> data;

    minecraft::protocol::writeString("minecraft:world_clock", data);

    minecraft::protocol::writeVarInt(1, data);

    minecraft::protocol::writeString("minecraft:overworld", data);

    // Optional NBT present
    data.push_back(0x01);

    auto clock = castle::nbt::Nbt::makeCompound("");

    std::vector<uint8_t> raw = castle::nbt::NbtWriter::writer(clock, {.namelessRoot = true});

    data.insert(data.end(), raw.begin(), raw.end());

    minecraft::protocol::sendPacket(
        connection, castle::protocol::ConfigurationClientbound::RegistryData, data);
  }
  void sendTimelineTags(csatle::network::TcpConnection& connection) {
    std::vector<uint8_t> data;

    // 1 tagged registry
    minecraft::protocol::writeVarInt(1, data);

    minecraft::protocol::writeString("minecraft:timeline", data);

    // 2 tags
    minecraft::protocol::writeVarInt(2, data);

    // minecraft:universal
    minecraft::protocol::writeString("minecraft:universal", data);

    minecraft::protocol::writeVarInt(1, data);
    minecraft::protocol::writeVarInt(3, data);

    // minecraft:in_overworld
    minecraft::protocol::writeString("minecraft:in_overworld", data);

    minecraft::protocol::writeVarInt(4, data);

    minecraft::protocol::writeVarInt(0, data); // day
    minecraft::protocol::writeVarInt(1, data); // early_game
    minecraft::protocol::writeVarInt(2, data); // moon
    minecraft::protocol::writeVarInt(3, data); // villager_schedule
                                               //

    minecraft::protocol::sendPacket(
        connection, castle::protocol::ConfigurationClientbound::UpdateTags, data);
  }

  void sendTimelineRegistry(csatle::network::TcpConnection& connection) {
    std::vector<uint8_t> data;

    minecraft::protocol::writeString("minecraft:timeline", data);

    minecraft::protocol::writeVarInt(4, data);

    // --------------------------------------------------
    // Entry 0: minecraft:day
    // --------------------------------------------------

    minecraft::protocol::writeString("minecraft:day", data);
    data.push_back(0x01);

    std::vector<uint8_t> rawDay;

    castle::io::appendsNbtFile("./generated/registries/timeline/day.nbt", rawDay);

    data.insert(data.end(), rawDay.begin(), rawDay.end());

    // --------------------------------------------------
    // Entry 1: minecraft:early_game
    // --------------------------------------------------

    minecraft::protocol::writeString("minecraft:early_game", data);

    data.push_back(0x01);

    std::vector<std::uint8_t> rawEarlyGame;

    castle::io::appendsNbtFile("./generated/registries/timeline/early_game.nbt", rawEarlyGame);

    data.insert(data.end(), rawEarlyGame.begin(), rawEarlyGame.end());

    // --------------------------------------------------
    // Entry 2: minecraft:moon
    // --------------------------------------------------

    minecraft::protocol::writeString("minecraft:moon", data);

    data.push_back(0x01);

    std::vector<std::uint8_t> rawMon;

    castle::io::appendsNbtFile("./generated/registries/timeline/moon.nbt", rawMon);

    data.insert(data.end(), rawMon.begin(), rawMon.end());

    // --------------------------------------------------
    // Entry 3: minecraft:villager_schedule
    // --------------------------------------------------

    minecraft::protocol::writeString("minecraft:villager_schedule", data);

    data.push_back(0x01);
    std::vector<std::uint8_t> rawVillager;

    castle::nbt::NbtReader::reader("./generated/registries/timeline/villager_schedule.nbt",
                                   rawVillager,
                                   {.namelessRoot = true});

    data.insert(data.end(), rawVillager.begin(), rawVillager.end());

    minecraft::protocol::sendPacket(
        connection, castle::protocol::ConfigurationClientbound::RegistryData, data);
  }
  void sendKnownPacks(csatle::network::TcpConnection& connection) {
    std::vector<uint8_t> data;

    // 1 known pack
    minecraft::protocol::writeVarInt(1, data);

    minecraft::protocol::writeString("minecraft", data);

    minecraft::protocol::writeString("core", data);

    minecraft::protocol::writeString("26.3", data);

    minecraft::protocol::sendPacket(
        connection, castle::protocol::ConfigurationClientbound::KnownPacks, data);
  }
  void sendTrimMaterials(csatle::network::TcpConnection& connection) {
    std::vector<std::uint8_t> data;

    minecraft::protocol::writeString("minecraft:trim_material", data);

    constexpr std::array variants = {"minecraft:quartz",
                                     "minecraft:iron",
                                     "minecraft:netherite",
                                     "minecraft:redstone",
                                     "minecraft:copper",
                                     "minecraft:gold",
                                     "minecraft:emerald",
                                     "minecraft:diamond",
                                     "minecraft:lapis",
                                     "minecraft:amethyst",
                                     "minecraft:resin"};

    minecraft::protocol::writeVarInt(variants.size(), data);

    for (const auto& variant : variants) {
      minecraft::protocol::writeString(variant, data);
      data.push_back(0x00);
    }

    minecraft::protocol::sendPacket(
        connection, castle::protocol::ConfigurationClientbound::RegistryData, data);
  }
  void blockTransformerEntrie(csatle::network::TcpConnection& connection) {
    std::vector<std::uint8_t> data;

    minecraft::protocol::writeString("minecraft:block_transformer", data);

    constexpr std::array variants = {"minecraft:axe", "minecraft:hoe", "minecraft:shovel"};

    minecraft::protocol::writeVarInt(variants.size(), data);

    for (const auto& variant : variants) {
      minecraft::protocol::writeString(variant, data);
      data.push_back(0x00);
    }

    minecraft::protocol::sendPacket(
        connection, castle::protocol::ConfigurationClientbound::RegistryData, data);
  }
} // namespace
using namespace minecraft::protocol;
namespace minecraft::protocol::handlers {
  void handleConfiguration(Client& client) {
    csatle::network::TcpConnection& connection = client.connection();

    std::vector<std::uint8_t> data = castle::network::receivePacket(connection);

    castle::protocol::PacketReader<castle::protocol::ConfigurationServerbound> packet(data);

    std::vector<std::uint8_t> temp;
    std::cout << "we now at: " << static_cast<std::int32_t>(packet.PacketId) << "\n";

    switch (packet.PacketId) {
    case castle::protocol::ConfigurationServerbound::ClientInfomation:
      sendKnownPacks(connection);
      break;
    case castle::protocol::ConfigurationServerbound::CookieResponse:
      break;
    case castle::protocol::ConfigurationServerbound::PluginMessage:
      break;
    case castle::protocol::ConfigurationServerbound::AcknowledgeFinishConfiguration:

      client.updateState(minecraft::protocol::State::Play);

      minecraft::protocol::handlers::initialiizePlay(client);
      break;
    case castle::protocol::ConfigurationServerbound::KeepAlive: {
      auto value = packet.getInt<std::int64_t>();
      // auto value = endian::readBigEndian<std::int64_t>(packet.data, offset);
      if (value != client.lastPing) {
        // close connection / time out
      }
      break;
    }
    case castle::protocol::ConfigurationServerbound::Pong:
      std::cout << "5\n";
      break;
    case castle::protocol::ConfigurationServerbound::ResourcePackResponse:
      std::cout << "6\n";
      break;
    case castle::protocol::ConfigurationServerbound::KnownPacks: {
      std::cout << "Known Packs response\n";

      auto count = packet.varInt();
      // count = 1, offset = 1

      auto nameSpace = packet.string();
      // nameSpace = "minecraft", offset = 11

      auto id = packet.string();
      // id = "core", offset = 16

      auto version = packet.string();

      std::cout << nameSpace << ":" << id << " version: " << version << "\n";

      sendWorldClockRegistry(connection);
      sendTimelineRegistry(connection);

      sendTrimMaterials(connection);
      blockTransformerEntrie(connection);
      sendEnty(connection); // dimension_type
      castle::entries::damgeTypesEntrie(connection);
      castle::registry::BannerPatternRegistry(connection);
      castle::registry::instrumentRegistry(connection);
      castle::registry::potteryEntrie(connection);
      castle::registry::biomeRegistry(connection);

      castle::entries::cowEntrie(connection);
      castle::entries::cowSoundEntrie(connection);
      castle::entries::catEntrie(connection);
      castle::entries::catSoundEntrie(connection);
      castle::entries::chickenEntrie(connection);
      castle::entries::chickenSoundEntrie(connection);

      castle::entries::frogEntrie(connection);
      castle::entries::paintingEntrie(connection);
      castle::entries::pigEntrie(connection);
      castle::entries::pigSoundEntrie(connection);
      castle::entries::wolfEntrie(connection);
      castle::entries::wolfSoundEntrie(connection);
      castle::entries::zombieEntrie(connection);

      castle::entries::jukeboxEntrie(connection);

      castle::entries::entriesTypes(connection);

      // cat_variant
      // cat_sound_variant
      // chicken_variant
      // etc.
      //

      sendTimelineTags(connection);

      //
      // Only after ALL registry data/tags have been sent:
      //
      minecraft::protocol::sendPacket(
          connection, ConfigurationClientbound::FinishConfiguration, {});

      break;
    }
    case castle::protocol::ConfigurationServerbound::CustomClickAction:
      // minecraft::protocol::readVarInt(connection);
      std::cout << "8\n";
      break;
    case castle::protocol::ConfigurationServerbound::AcceptCodeOfConduct:
      // minecraft::protocol::readVarInt(connection);
      std::cout << "9\n";
      break;
    default:
      std::cerr << "Unknown Configuration packet: 0x" << std::hex
                << static_cast<std::int32_t>(packet.PacketId) << std::dec << '\n';
      break;
      break;
    }
  }
} // namespace minecraft::protocol::handlers
