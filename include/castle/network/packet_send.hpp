#pragma once

#include "castle/network/tcp_connection.hpp"
#include "castleProtocol/package/packet.hpp"
#include "castleProtocol/package/writePacket.hpp"
namespace castle::network {
  template <castle::protocol::Enum E>
  void sendPacket(castle::network::TcpConnection& connection,
                  const protocol::PacketWriter<E>& packet) {
    auto data = packet.frame();
    connection.writeAll(data.data(), data.size());
  }
} // namespace castle::network
