#pragma once

#include "castle/network/tcp_connection.hpp"
#include <cstdint>
#include <span>
#include <string>
#include <type_traits>
#include <vector>

class Client;
namespace minecraft::protocol {

  int32_t readVarInt(csatle::network::TcpConnection& connection);
  int32_t readVarInt(std::span<const std::uint8_t> data, std::size_t& offset);
  void writeVarInt(int32_t value, std::vector<uint8_t>& output);
  std::string readString(csatle::network::TcpConnection& connection);
  std::string readString(std::span<const std::uint8_t> data, std::size_t& offset);
  void writeString(const std::string& text, std::vector<uint8_t>& output);

  void handleClient(Client& client);

} // namespace minecraft::protocol
