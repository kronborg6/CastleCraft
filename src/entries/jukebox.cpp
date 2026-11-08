#include "entries/jukebox.hpp"
#include "castle/network/packet_send.hpp"
#include "castle/network/tcp_connection.hpp"
#include "castleProtocol/protocol/configuration.hpp"

namespace castle::entries {
using ConfigurationPacket =
    castle::protocol::PacketWriter<castle::protocol::ConfigurationClientbound>;

void jukeboxEntrie(csatle::network::TcpConnection &connection) {
	constexpr std::array variants = {"minecraft:11",
	                                 "minecraft:13",
	                                 "minecraft:5",
	                                 "minecraft:blocks",
	                                 "minecraft:bounce",
	                                 "minecraft:cat",
	                                 "minecraft:chirp",
	                                 "minecraft:creator",
	                                 "minecraft:creator_music_box",
	                                 "minecraft:far",
	                                 "minecraft:lava_chicken",
	                                 "minecraft:mall",
	                                 "minecraft:mellohi",
	                                 "minecraft:otherside",
	                                 "minecraft:pigstep",
	                                 "minecraft:precipice",
	                                 "minecraft:relic",
	                                 "minecraft:stal",
	                                 "minecraft:strad",
	                                 "minecraft:tears",
	                                 "minecraft:wait",
	                                 "minecraft:ward"};

	ConfigurationPacket packet(
	    castle::protocol::ConfigurationClientbound::RegistryData);

	packet.addString("minecraft:jukebox_song");

	// Number of entries
	packet.addVarInt(static_cast<std::int32_t>(variants.size()));

	for (const auto &name : variants) {

		packet.addString(name);

		packet.addByte(0x00);
	}

	castle::network::sendPacket(connection, packet);
}
} // namespace castle::entries
