#include "handlers/configuration.hpp"
#include "castle/nbt.hpp"
#include "castle/network/packet_receiver.hpp"
#include "castle/network/packet_send.hpp"
#include "castle/network/tcp_connection.hpp"
#include "castle/reader.hpp"
#include "castle/writer.hpp"
#include "castleProtocol/package/writePacket.hpp"
#include "client.hpp"
#include "entries/damge_types.hpp"
#include "entries/entries.hpp"
#include "entries/jukebox.hpp"
#include "entries/mobs.hpp"
#include "handlers/play.hpp"
#include "io/nbt_loader.hpp"
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
using ConfigurationPacket =
    castle::protocol::PacketWriter<castle::protocol::ConfigurationClientbound>;

void sendEnty(csatle::network::TcpConnection &connection) {
	std::vector<uint8_t> rawNbt;

	castle::io::appendsNbtFile("./generated/dimensions/overworld.nbt", rawNbt);

	ConfigurationPacket packet(
	    castle::protocol::ConfigurationClientbound::RegistryData);

	packet.addString("minecraft:dimension_type");

	packet.addVarInt(1);

	packet.addString("minecraft:overworld");

	packet.addByte(0x01);

	packet.addBytes(rawNbt);
	castle::network::sendPacket(connection, packet);
}
void sendWorldClockRegistry(csatle::network::TcpConnection &connection) {
	auto clock = castle::nbt::Nbt::makeCompound("");

	std::vector<uint8_t> raw =
	    castle::nbt::NbtWriter::writer(clock, {.namelessRoot = true});

	ConfigurationPacket packet(
	    castle::protocol::ConfigurationClientbound::RegistryData);

	packet.addString("minecraft:world_clock");

	packet.addVarInt(1);

	packet.addString("minecraft:overworld");

	packet.addByte(0x01);

	packet.addBytes(raw);
	castle::network::sendPacket(connection, packet);
}
void sendTimelineTags(csatle::network::TcpConnection &connection) {
	ConfigurationPacket packet(
	    castle::protocol::ConfigurationClientbound::UpdateTags);

	packet.addVarInt(1);

	packet.addString("minecraft:timeline");

	packet.addVarInt(2);

	packet.addString("minecraft:universal");

	packet.addVarInt(1);

	packet.addVarInt(3);

	packet.addString("minecraft:in_overworld");

	packet.addVarInt(4);

	packet.addVarInt(0);
	packet.addVarInt(1);
	packet.addVarInt(2);
	packet.addVarInt(3);
	castle::network::sendPacket(connection, packet);
}

void sendTimelineRegistry(csatle::network::TcpConnection &connection) {
	ConfigurationPacket packet(
	    castle::protocol::ConfigurationClientbound::RegistryData);

	packet.addString("minecraft:timeline");

	packet.addVarInt(4);

	// --------------------------------------------------
	// Entry 0: minecraft:day
	// --------------------------------------------------

	std::vector<uint8_t> rawDay;
	castle::io::appendsNbtFile("./generated/registries/timeline/day.nbt",
	                           rawDay);
	packet.addString("minecraft:day");

	packet.addByte(0x01);

	packet.addBytes(rawDay);

	// --------------------------------------------------
	// Entry 1: minecraft:early_game
	// --------------------------------------------------

	std::vector<std::uint8_t> rawEarlyGame;
	castle::io::appendsNbtFile("./generated/registries/timeline/early_game.nbt",
	                           rawEarlyGame);
	packet.addString("minecraft:early_game");

	packet.addByte(0x01);

	packet.addBytes(rawEarlyGame);

	// --------------------------------------------------
	// Entry 2: minecraft:moon
	// --------------------------------------------------

	std::vector<std::uint8_t> rawMon;
	castle::io::appendsNbtFile("./generated/registries/timeline/moon.nbt",
	                           rawMon);
	packet.addString("minecraft:moon");

	packet.addByte(0x01);

	packet.addBytes(rawMon);

	// --------------------------------------------------
	// Entry 3: minecraft:villager_schedule
	// --------------------------------------------------

	std::vector<std::uint8_t> rawVillager;

	castle::nbt::NbtReader::reader(
	    "./generated/registries/timeline/villager_schedule.nbt", rawVillager,
	    {.namelessRoot = true});

	packet.addString("minecraft:villager_schedule");

	packet.addByte(0x01);

	packet.addBytes(rawVillager);
	castle::network::sendPacket(connection, packet);
}
void sendKnownPacks(csatle::network::TcpConnection &connection) {
	ConfigurationPacket packet(
	    castle::protocol::ConfigurationClientbound::KnownPacks);

	packet.addVarInt(1);

	packet.addString("minecraft");

	packet.addString("core");

	packet.addString("26.3");
	castle::network::sendPacket(connection, packet);
}
void sendTrimMaterials(csatle::network::TcpConnection &connection) {
	constexpr std::array variants = {
	    "minecraft:quartz",   "minecraft:iron",    "minecraft:netherite",
	    "minecraft:redstone", "minecraft:copper",  "minecraft:gold",
	    "minecraft:emerald",  "minecraft:diamond", "minecraft:lapis",
	    "minecraft:amethyst", "minecraft:resin"};

	ConfigurationPacket packet(
	    castle::protocol::ConfigurationClientbound::RegistryData);

	packet.addString("minecraft:trim_material");

	packet.addVarInt(static_cast<std::int32_t>(variants.size()));
	for (const auto &variant : variants) {
		packet.addString(variant);

		packet.addByte(0x00);
	}
	castle::network::sendPacket(connection, packet);
}
void blockTransformerEntrie(csatle::network::TcpConnection &connection) {
	constexpr std::array variants = {"minecraft:axe", "minecraft:hoe",
	                                 "minecraft:shovel"};

	ConfigurationPacket packet(
	    castle::protocol::ConfigurationClientbound::RegistryData);

	packet.addString("minecraft:block_transformer");

	packet.addVarInt(static_cast<std::int32_t>(variants.size()));
	for (const auto &variant : variants) {
		packet.addString(variant);

		packet.addByte(0x00);
	}
	castle::network::sendPacket(connection, packet);
}
} // namespace
using namespace minecraft::protocol;
namespace minecraft::protocol::handlers {
void handleConfiguration(Client &client) {
	csatle::network::TcpConnection &connection = client.connection();

	std::vector<std::uint8_t> data = castle::network::receivePacket(connection);

	castle::protocol::PacketReader<castle::protocol::ConfigurationServerbound>
	    packet(data);

	std::vector<std::uint8_t> temp;
	std::cout << "we now at: " << static_cast<std::int32_t>(packet.PacketId)
	          << "\n";

	switch (packet.PacketId) {
	case castle::protocol::ConfigurationServerbound::ClientInfomation:
		sendKnownPacks(connection);
		break;
	case castle::protocol::ConfigurationServerbound::CookieResponse:
		break;
	case castle::protocol::ConfigurationServerbound::PluginMessage:
		break;
	case castle::protocol::ConfigurationServerbound::
	    AcknowledgeFinishConfiguration:

		client.updateState(minecraft::protocol::State::Play);

		minecraft::protocol::handlers::initialiizePlay(client);
		break;
	case castle::protocol::ConfigurationServerbound::KeepAlive: {
		auto value = packet.getInt<std::int64_t>();
		// auto value = endian::readBigEndian<std::int64_t>(packet.data,
		// offset);
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
		castle::protocol::PacketWriter<
		    castle::protocol::ConfigurationClientbound>
		    finishPacket(castle::protocol::ConfigurationClientbound::
		                     FinishConfiguration);
		castle::network::sendPacket(connection, finishPacket);

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
		          << static_cast<std::int32_t>(packet.PacketId) << std::dec
		          << '\n';
		break;
		break;
	}
}
} // namespace minecraft::protocol::handlers
