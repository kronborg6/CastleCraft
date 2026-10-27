#include "handlers/configuration.hpp"
#include "castle/nbt.hpp"
#include "castle/reader.hpp"
#include "castle/writer.hpp"
#include "client.hpp"
#include "io/nbt_loader.hpp"
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
    std::vector<uint8_t> rawNbt;

    castle::io::appendsNbtFile("./generated/dimensions/overworld.nbt", rawNbt);

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

    size_t size4 =
        castle::nbt::NbtReader::reader("./generated/registries/timeline/villager_schedule.nbt",
                                       rawVillager,
                                       {.namelessRoot = true});

    data.insert(data.end(), rawVillager.begin(), rawVillager.end());

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

    switch (packet.packetId) {
    case 0x00:
      // minecraft::protocol::readVarInt(connection);
      sendKnownPacks(connection);
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

      std::cout << "aarhh\n";
      std::cout << "aarhh\n";
      std::cout << "aarhh\n";
      std::cout << "aarhh\n";
      std::cout << "aarhh\n";
      std::cout << "aarhh\n";
      std::cout << "aarhh\n";

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
