#include "castle/network/varint.hpp"
#include "castle/network/tcp_connection.hpp"
#include <cstdint>
#include <stdexcept>

namespace castle::network {
std::int32_t readVarInt(castle::network::TcpConnection &connection) {
  int32_t value = 0;
  int position = 0;

  while (true) {
    const uint8_t byte = connection.readByte();

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
} // namespace castle::network
