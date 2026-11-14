#include "castle/handlers/play/tabList.hpp"
#include "castle/handlers/play/play.hpp"

namespace castle::handlers::play {
  PlayPacketWriter playerInfoUpdate(Client& client, player::PlayerAction action) {
    PlayPacketWriter packet(protocol::PlayClientbound::PlayerInfoUpdate);

    const auto& profile = client.profile;

    std::uint8_t actions = 0;

    switch (action) {
    case player::PlayerAction::AddPlayer:
      actions = 0x01 | 0x04 | 0x08 | 0x10;
      break;

    case player::PlayerAction::UpdateGameMode:
      actions = 0x04;
      break;

    case player::PlayerAction::UpdateListed:
      actions = 0x08;
      break;

    case player::PlayerAction::UpdateLatency:
      actions = 0x10;
      break;
    default:
      throw std::runtime_error(
          std::format("Not supported action: 0x{:02X}", static_cast<unsigned>(action)));
    }

    packet.addByte(actions);
    packet.addVarInt(1); // player cout need to take for som where else
    packet.addBytes(profile.uuid);

    // Write fields in protocol action-bit order
    if (actions & 0x01) {
      packet.addString(profile.username); // use create a nickname fields and use that insted?
      packet.addVarInt(0);                // Properties
    }

    if (actions & 0x04) {
      packet.addVarInt(1); // take from client insted?
    }

    if (actions & 0x08) {
      packet.addByte(1); // hide: 0 show: 1 in the tab take from player to or somfing
    }

    if (actions & 0x10) {
      packet.addVarInt(0); // use last ping to calulte or somfing
    }

    return packet;
    // network::sendPacket(client.connection(), packet);
    //
    // client.broadcast(packet);
  }
} // namespace castle::handlers::play
