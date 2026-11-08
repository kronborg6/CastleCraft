#include "castle/entries/entries.hpp"
#include "castle/entries/banner.hpp"
#include "castle/entries/damge_types.hpp"
#include "castle/network/packet_send.hpp"
#include "castle/network/tcp_connection.hpp"
#include "castleProtocol/protocol/configuration.hpp"
namespace castle::entries {
  using ConfigurationPacket =
      castle::protocol::PacketWriter<castle::protocol::ConfigurationClientbound>;

  void entriesTypes(csatle::network::TcpConnection& connection) {
    ConfigurationPacket packet(castle::protocol::ConfigurationClientbound::UpdateTags);

    packet.addVarInt(3);

    packet.addString("minecraft:damage_type");

    // Number of tags
    packet.addVarInt(3);

    explosionTypesEntrie(packet);
    fireTypesEntrie(packet);
    bypassShieldTypesEntrie(packet);

    packet.addString("minecraft:timeline");

    // Number of tags
    packet.addVarInt(2);

    // Tag 0: minecraft:universal
    packet.addString("minecraft:universal");

    packet.addVarInt(1);

    packet.addVarInt(3);

    // Tag 1: minecraft:in_overworld
    packet.addString("minecraft:in_overworld");

    packet.addVarInt(4);

    for (std::int32_t id = 0; id < 4; ++id) {

      packet.addVarInt(id);
    }

    bannerTagsEntrie(packet);

    // Send Update Tags
    castle::network::sendPacket(connection, packet);
  }
} // namespace castle::entries
