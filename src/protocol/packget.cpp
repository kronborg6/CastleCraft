#include "protocol/packget.hpp"
#include "protocol/protocol.hpp"
#include "tcp.hpp"
#include <cstddef>
#include <cstdint>
#include <stdexcept>
#include <vector>
namespace minecraft::protocol {

  Packget createPacket(TcpConnection& connection) {
    const int32_t packetLength = readVarInt(connection);

    const int32_t packetId = readVarInt(connection);

    if (packetLength < 1) {
      throw std::runtime_error("Invalid packet length");
    }

    const std::size_t payloadLength = static_cast<std::size_t>(packetLength - 1);

    std::vector<std::uint8_t> data(payloadLength);

    connection.readAll(data.data(), data.size());

    return Packget{packetId, std::move(data)};
  }
} // namespace minecraft::protocol
