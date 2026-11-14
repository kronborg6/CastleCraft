#include "castle/network/packet_receiver.hpp"
#include "castle/network/tcp_connection.hpp"
#include "castle/network/varint.hpp"
#include <stdexcept>

namespace castle::network {
std::vector<std::uint8_t>
receivePacket(castle::network::TcpConnection &connection) {

  const std::int32_t length = readVarInt(connection);

  if (length <= 0 || length > MAX_PACKET_SIZE) {
    throw std::runtime_error("Invalid packet length");
  }

  std::vector<std::uint8_t> data(static_cast<std::size_t>(length));

  connection.readAll(data.data(), data.size());

  return data;
}

} // namespace castle::network
