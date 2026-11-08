#include "registry/biome.hpp"
#include "castle/network/packet_send.hpp"
#include "castle/network/tcp_connection.hpp"
#include "castleProtocol/protocol/configuration.hpp"
#include <cstdint>
#include <vector>
namespace castle::registry {
using ConfigurationPacket =
    castle::protocol::PacketWriter<castle::protocol::ConfigurationClientbound>;

void biomeRegistry(csatle::network::TcpConnection &connection) {
	ConfigurationPacket packet(
	    castle::protocol::ConfigurationClientbound::RegistryData);

	packet.addString("minecraft:worldgen/biome");

	packet.addVarInt(static_cast<std::int32_t>(BIOME_TYPES.size()));

	for (const auto &name : BIOME_TYPES) {

		packet.addString(name);

		packet.addByte(0x00);
	}
	castle::network::sendPacket(connection, packet);
}
} // namespace castle::registry
