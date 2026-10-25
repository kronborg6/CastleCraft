#include "handlers/configuration.hpp"
#include "castle/nbt.hpp"
#include "castle/writer.hpp"
#include "client.hpp"
#include "protocol/protocol.hpp"
#include "tcp.hpp"
#include <cstdint>
#include <iostream>
#include <vector>

namespace {
  int32_t readVarInt2(std::vector<std::uint8_t>& data) {
    int32_t value = 0;
    int position = 0;

    while (true) {
      const uint8_t byte = data.at(1);

      value |= (byte & 0x7F) << position;

      if ((byte & 0x80) == 0) {
        break;
      }

      position += 7;

      if (position >= 32) {
        throw std::runtime_error("VarInt too large");
      }
    }

    return value;
  }
  struct Packget {
      int32_t packetId;
      std::vector<std::uint8_t> data;
  };
  Packget createPacket(TcpConnection& connection) {
    const int32_t packetLength = minecraft::protocol::readVarInt(connection);

    const int32_t packetId = minecraft::protocol::readVarInt(connection);

    if (packetLength < 1) {
      throw std::runtime_error("Invalid packet length");
    }

    const std::size_t payloadLength = static_cast<std::size_t>(packetLength - 1);

    std::vector<std::uint8_t> data(payloadLength);

    connection.readAll(data.data(), data.size());

    return Packget{packetId, std::move(data)};
  }
  void sendEnty(TcpConnection& connection) {
    std::vector<uint8_t> data;

    auto root = castle::nbt::Nbt::makeCompound("");

    root.add(castle::nbt::Nbt::makeFloat("ambient_light", 0.0f));

    root.add(castle::nbt::Nbt::makeCompound(
        "attributes",
        {castle::nbt::Nbt::makeCompound(
             "minecraft:audio/ambient_sounds",
             {castle::nbt::Nbt::makeCompound(
                 "mood",
                 {castle::nbt::Nbt::makeInt("block_search_extent", 8),
                  castle::nbt::Nbt::makeDouble("offset", 2.0),
                  castle::nbt::Nbt::makeString("sound", "minecraft:ambient.cave"),
                  castle::nbt::Nbt::makeInt("tick_delay", 6000)})}),

         castle::nbt::Nbt::makeCompound(
             "minecraft:audio/background_music",
             {castle::nbt::Nbt::makeCompound(
                  "creative",
                  {castle::nbt::Nbt::makeInt("max_delay", 24000),
                   castle::nbt::Nbt::makeInt("min_delay", 12000),
                   castle::nbt::Nbt::makeString("sound", "minecraft:music.creative")}),

              castle::nbt::Nbt::makeCompound(
                  "default",
                  {castle::nbt::Nbt::makeInt("max_delay", 24000),
                   castle::nbt::Nbt::makeInt("min_delay", 12000),
                   castle::nbt::Nbt::makeString("sound", "minecraft:music.game")})}),

         castle::nbt::Nbt::makeString("minecraft:visual/ambient_light_color", "#0a0a0a"),

         castle::nbt::Nbt::makeString("minecraft:visual/cloud_color", "#ccffffff"),

         castle::nbt::Nbt::makeFloat("minecraft:visual/cloud_height", 192.33f),

         castle::nbt::Nbt::makeString("minecraft:visual/fog_color", "#c0d8ff"),

         castle::nbt::Nbt::makeString("minecraft:visual/sky_color", "#78a7ff")}));
    root.add(castle::nbt::Nbt::makeDouble("coordinate_scale", 1.0));

    root.add(castle::nbt::Nbt::makeString("default_clock", "minecraft:overworld"));

    root.add(castle::nbt::Nbt::makeByte("has_ceiling", 0));

    root.add(castle::nbt::Nbt::makeByte("has_ender_dragon_fight", 0));

    root.add(castle::nbt::Nbt::makeByte("has_skylight", 1));

    root.add(castle::nbt::Nbt::makeInt("height", 384));

    root.add(castle::nbt::Nbt::makeString("infiniburn", "#minecraft:infiniburn_overworld"));

    root.add(castle::nbt::Nbt::makeInt("logical_height", 384));

    root.add(castle::nbt::Nbt::makeInt("min_y", -64));

    root.add(castle::nbt::Nbt::makeInt("monster_spawn_block_light_limit", 0));

    root.add(
        castle::nbt::Nbt::makeCompound("monster_spawn_light_level",
                                       {castle::nbt::Nbt::makeString("type", "minecraft:uniform"),

                                        castle::nbt::Nbt::makeInt("max_inclusive", 7),

                                        castle::nbt::Nbt::makeInt("min_inclusive", 0)}));
    root.add(
        castle::nbt::Nbt::makeList("infiniburn",
                                   {castle::nbt::Nbt::makeString("", "minecraft:magma_block"),
                                    castle::nbt::Nbt::makeString("", "minecraft:netherrack")}));
    root.add(castle::nbt::Nbt::makeString("timelines", "#minecraft:in_overworld"));

    std::vector<uint8_t> rawNbt = castle::nbt::NbtWriter::writer(root, {.namelessRoot = true});
    minecraft::protocol::writeString("minecraft:dimension_type", data);

    // zero entries
    minecraft::protocol::writeVarInt(1, data);

    minecraft::protocol::writeString("minecraft:overworld", data);

    data.push_back(0x01);

    data.insert(data.end(), rawNbt.begin(), rawNbt.end());

    minecraft::protocol::sendPacket(connection, 0x07, data);
  }
  void sendWorldClockRegistry(TcpConnection& connection) {
    std::vector<uint8_t> data;

    minecraft::protocol::writeString("minecraft:world_clock", data);

    minecraft::protocol::writeVarInt(1, data);

    minecraft::protocol::writeString("minecraft:overworld", data);

    // Optional NBT present
    data.push_back(0x01);

    auto clock = castle::nbt::Nbt::makeCompound("");

    std::vector<uint8_t> raw = castle::nbt::NbtWriter::writer(clock, {.namelessRoot = true});

    // network NBT: remove root name bytes if your writer
    // still produces 0A 00 00
    // or preferably have your writer support network mode.

    data.insert(data.end(), raw.begin(), raw.end());

    minecraft::protocol::sendPacket(connection, 0x07, data);
  }
  void sendTimelineTags(TcpConnection& connection) {
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

    minecraft::protocol::sendPacket(connection, 0x0E, data);
  }
  void sendTimelineRegistry(TcpConnection& connection) {
    std::vector<uint8_t> data;

    minecraft::protocol::writeString("minecraft:timeline", data);

    minecraft::protocol::writeVarInt(4, data);

    // --------------------------------------------------
    // Entry 0: minecraft:day
    // --------------------------------------------------

    minecraft::protocol::writeString("minecraft:day", data);
    data.push_back(0x01);

    auto day = castle::nbt::Nbt::makeCompound(
        "",
        {castle::nbt::Nbt::makeString("clock", "minecraft:overworld"),

         castle::nbt::Nbt::makeInt("period_ticks", 24000),

         castle::nbt::Nbt::makeCompound("tracks", {})});

    auto dayRaw = castle::nbt::NbtWriter::writer(day, {.namelessRoot = true});

    data.insert(data.end(), dayRaw.begin(), dayRaw.end());

    // --------------------------------------------------
    // Entry 1: minecraft:early_game
    // --------------------------------------------------

    minecraft::protocol::writeString("minecraft:early_game", data);

    data.push_back(0x01);

    auto earlyGame = castle::nbt::Nbt::makeCompound(
        "",
        {castle::nbt::Nbt::makeString("clock", "minecraft:overworld"),

         castle::nbt::Nbt::makeCompound(
             "tracks",
             {castle::nbt::Nbt::makeCompound(
                 "minecraft:gameplay/can_pillager_patrol_spawn",
                 {castle::nbt::Nbt::makeList(
                      "keyframes",
                      {castle::nbt::Nbt::makeCompound("",
                                                      {castle::nbt::Nbt::makeInt("ticks", 0),
                                                       castle::nbt::Nbt::makeByte("value", 0)}),

                       castle::nbt::Nbt::makeCompound("",
                                                      {castle::nbt::Nbt::makeInt("ticks", 120000),
                                                       castle::nbt::Nbt::makeByte("value", 1)})}),

                  castle::nbt::Nbt::makeString("modifier", "and")})})});

    auto earlyGameRaw = castle::nbt::NbtWriter::writer(earlyGame, {.namelessRoot = true});

    data.insert(data.end(), earlyGameRaw.begin(), earlyGameRaw.end());

    // --------------------------------------------------
    // Entry 2: minecraft:moon
    // --------------------------------------------------

    minecraft::protocol::writeString("minecraft:moon", data);

    data.push_back(0x01);

    auto moon = castle::nbt::Nbt::makeCompound(
        "",
        {castle::nbt::Nbt::makeString("clock", "minecraft:overworld"),

         castle::nbt::Nbt::makeInt("period_ticks", 192000),

         castle::nbt::Nbt::makeCompound(
             "tracks",
             {castle::nbt::Nbt::makeCompound(
                  "minecraft:gameplay/surface_slime_spawn_chance",
                  {castle::nbt::Nbt::makeString("ease", "constant"),

                   castle::nbt::Nbt::makeList(
                       "keyframes",
                       {castle::nbt::Nbt::makeCompound(
                            "",
                            {castle::nbt::Nbt::makeInt("ticks", 0),
                             castle::nbt::Nbt::makeDouble("value", 0.5)}),
                        castle::nbt::Nbt::makeCompound(
                            "",
                            {castle::nbt::Nbt::makeInt("ticks", 24000),
                             castle::nbt::Nbt::makeDouble("value", 0.375)}),
                        castle::nbt::Nbt::makeCompound(
                            "",
                            {castle::nbt::Nbt::makeInt("ticks", 48000),
                             castle::nbt::Nbt::makeDouble("value", 0.25)}),
                        castle::nbt::Nbt::makeCompound(
                            "",
                            {castle::nbt::Nbt::makeInt("ticks", 72000),
                             castle::nbt::Nbt::makeDouble("value", 0.125)}),
                        castle::nbt::Nbt::makeCompound(
                            "",
                            {castle::nbt::Nbt::makeInt("ticks", 96000),
                             castle::nbt::Nbt::makeDouble("value", 0.0)}),
                        castle::nbt::Nbt::makeCompound(
                            "",
                            {castle::nbt::Nbt::makeInt("ticks", 120000),
                             castle::nbt::Nbt::makeDouble("value", 0.125)}),
                        castle::nbt::Nbt::makeCompound(
                            "",
                            {castle::nbt::Nbt::makeInt("ticks", 144000),
                             castle::nbt::Nbt::makeDouble("value", 0.25)}),
                        castle::nbt::Nbt::makeCompound(
                            "",
                            {castle::nbt::Nbt::makeInt("ticks", 168000),
                             castle::nbt::Nbt::makeDouble("value", 0.375)})}),

                   castle::nbt::Nbt::makeString("modifier", "maximum")}),

              castle::nbt::Nbt::makeCompound(
                  "minecraft:visual/moon_phase",
                  {castle::nbt::Nbt::makeList(
                      "keyframes",
                      {castle::nbt::Nbt::makeCompound(
                           "",
                           {castle::nbt::Nbt::makeInt("ticks", 0),
                            castle::nbt::Nbt::makeString("value", "full_moon")}),
                       castle::nbt::Nbt::makeCompound(
                           "",
                           {castle::nbt::Nbt::makeInt("ticks", 24000),
                            castle::nbt::Nbt::makeString("value", "waning_gibbous")}),
                       castle::nbt::Nbt::makeCompound(
                           "",
                           {castle::nbt::Nbt::makeInt("ticks", 48000),
                            castle::nbt::Nbt::makeString("value", "third_quarter")}),
                       castle::nbt::Nbt::makeCompound(
                           "",
                           {castle::nbt::Nbt::makeInt("ticks", 72000),
                            castle::nbt::Nbt::makeString("value", "waning_crescent")}),
                       castle::nbt::Nbt::makeCompound(
                           "",
                           {castle::nbt::Nbt::makeInt("ticks", 96000),
                            castle::nbt::Nbt::makeString("value", "new_moon")}),
                       castle::nbt::Nbt::makeCompound(
                           "",
                           {castle::nbt::Nbt::makeInt("ticks", 120000),
                            castle::nbt::Nbt::makeString("value", "waxing_crescent")}),
                       castle::nbt::Nbt::makeCompound(
                           "",
                           {castle::nbt::Nbt::makeInt("ticks", 144000),
                            castle::nbt::Nbt::makeString("value", "first_quarter")}),
                       castle::nbt::Nbt::makeCompound(
                           "",
                           {castle::nbt::Nbt::makeInt("ticks", 168000),
                            castle::nbt::Nbt::makeString("value", "waxing_gibbous")})})})})});

    auto moonRaw = castle::nbt::NbtWriter::writer(moon, {.namelessRoot = true});

    data.insert(data.end(), moonRaw.begin(), moonRaw.end());

    // --------------------------------------------------
    // Entry 3: minecraft:villager_schedule
    // --------------------------------------------------

    minecraft::protocol::writeString("minecraft:villager_schedule", data);

    data.push_back(0x01);

    auto villagerSchedule = castle::nbt::Nbt::makeCompound(
        "",
        {castle::nbt::Nbt::makeString("clock", "minecraft:overworld"),

         castle::nbt::Nbt::makeInt("period_ticks", 24000),

         castle::nbt::Nbt::makeCompound(
             "tracks",
             {castle::nbt::Nbt::makeCompound(
                  "minecraft:gameplay/baby_villager_activity",
                  {castle::nbt::Nbt::makeList(
                      "keyframes",
                      {castle::nbt::Nbt::makeCompound(
                           "",
                           {castle::nbt::Nbt::makeInt("ticks", 10),
                            castle::nbt::Nbt::makeString("value", "minecraft:idle")}),
                       castle::nbt::Nbt::makeCompound(
                           "",
                           {castle::nbt::Nbt::makeInt("ticks", 3000),
                            castle::nbt::Nbt::makeString("value", "minecraft:play")}),
                       castle::nbt::Nbt::makeCompound(
                           "",
                           {castle::nbt::Nbt::makeInt("ticks", 6000),
                            castle::nbt::Nbt::makeString("value", "minecraft:idle")}),
                       castle::nbt::Nbt::makeCompound(
                           "",
                           {castle::nbt::Nbt::makeInt("ticks", 10000),
                            castle::nbt::Nbt::makeString("value", "minecraft:play")}),
                       castle::nbt::Nbt::makeCompound(
                           "",
                           {castle::nbt::Nbt::makeInt("ticks", 12000),
                            castle::nbt::Nbt::makeString("value", "minecraft:rest")})})}),

              castle::nbt::Nbt::makeCompound(
                  "minecraft:gameplay/villager_activity",
                  {castle::nbt::Nbt::makeList(
                      "keyframes",
                      {castle::nbt::Nbt::makeCompound(
                           "",
                           {castle::nbt::Nbt::makeInt("ticks", 10),
                            castle::nbt::Nbt::makeString("value", "minecraft:idle")}),
                       castle::nbt::Nbt::makeCompound(
                           "",
                           {castle::nbt::Nbt::makeInt("ticks", 2000),
                            castle::nbt::Nbt::makeString("value", "minecraft:work")}),
                       castle::nbt::Nbt::makeCompound(
                           "",
                           {castle::nbt::Nbt::makeInt("ticks", 9000),
                            castle::nbt::Nbt::makeString("value", "minecraft:meet")}),
                       castle::nbt::Nbt::makeCompound(
                           "",
                           {castle::nbt::Nbt::makeInt("ticks", 11000),
                            castle::nbt::Nbt::makeString("value", "minecraft:idle")}),
                       castle::nbt::Nbt::makeCompound(
                           "",
                           {castle::nbt::Nbt::makeInt("ticks", 12000),
                            castle::nbt::Nbt::makeString("value", "minecraft:rest")})})})})});

    auto villagerRaw = castle::nbt::NbtWriter::writer(villagerSchedule, {.namelessRoot = true});

    data.insert(data.end(), villagerRaw.begin(), villagerRaw.end());

    minecraft::protocol::sendPacket(connection, 0x07, data);
  }
  void sendKnownPacks(TcpConnection& connection) {
    std::vector<uint8_t> data;

    // 1 known pack
    minecraft::protocol::writeVarInt(1, data);

    minecraft::protocol::writeString("minecraft", data);

    minecraft::protocol::writeString("core", data);

    minecraft::protocol::writeString("26.3", data);

    minecraft::protocol::sendPacket(connection, 0x0F, data);
  }
} // namespace

namespace minecraft::protocol::handlers {
  void handleConfiguration(Client& client) {
    TcpConnection& connection = client.connection();

    Packget packet = createPacket(connection);

    std::vector<std::uint8_t> temp;
    std::cout << "we now at: " << packet.packetId << "\n";

    if (client.sent == false) {
      sendWorldClockRegistry(connection);
      sendTimelineRegistry(connection);
      sendEnty(connection); // dimension_type
      sendTimelineTags(connection);
      sendKnownPacks(connection);
      minecraft::protocol::sendPacket(connection, 0x03, {});
      client.sent = true;
    }

    switch (packet.packetId) {
    case 0x00:
      // minecraft::protocol::readVarInt(connection);
      std::cout << "0\n";
      break;
    case 0x01:
      // minecraft::protocol::readVarInt(connection);
      std::cout << "1\n";
      break;
    case 0x02:
      // minecraft::protocol::readVarInt(connection);
      std::cout << "2\n";
      break;
    case 0x03:
      // minecraft::protocol::sendPacket(connection, 0x03, {});
      client.updateState(minecraft::protocol::State::Play);
      std::cout << "3\n";
      break;
    case 0x04:
      // minecraft::protocol::readVarInt(connection);
      std::cout << "4\n";
      break;
    case 0x05:
      // int gg = readVarInt2(packet.data);

      // minecraft::protocol::writeVarInt(0, temp);
      // minecraft::protocol::sendPacket(connection, 0x05, temp);
      std::cout << "5\n";
      break;
    case 0x06:
      // minecraft::protocol::readVarInt(connection);
      std::cout << "6\n";
      break;
    case 0x07: {
      std::cout << "Known Packs response\n";

      //
      // NOW the Known Packs negotiation is finished.
      //
      // Next send the registries.
      //

      sendWorldClockRegistry(connection);
      sendTimelineRegistry(connection);
      sendEnty(connection); // dimension_type

      //
      // TODO:
      // Send the other required registries here.
      //
      // cat_variant
      // cat_sound_variant
      // chicken_variant
      // etc.
      //

      sendTimelineTags(connection);

      //
      // Only after ALL registry data/tags have been sent:
      //
      minecraft::protocol::sendPacket(connection, 0x03, {});

      break;
    }
    case 0x08:
      // minecraft::protocol::readVarInt(connection);
      std::cout << "8\n";
      break;
    case 0x09:
      // minecraft::protocol::readVarInt(connection);
      std::cout << "9\n";
      break;
    default:
      std::cerr << "Unknown Configuration packet: 0x" << std::hex << packet.packetId << std::dec
                << '\n';
      break;
      break;
    }
  }
} // namespace minecraft::protocol::handlers
