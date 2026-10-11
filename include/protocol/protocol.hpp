#pragma once

#include "tcp.hpp"
#include <cstdint>
#include <span>
#include <string>
#include <vector>

class Client;
class Client;
namespace minecraft::protocol {

int32_t readVarInt(TcpConnection &connection);
void writeVarInt(int32_t value, std::vector<uint8_t> &output);
std::string readString(TcpConnection &connection);
void writeString(const std::string &text, std::vector<uint8_t> &output);
void sendPacket(TcpConnection &connection, int32_t packetId,
                const std::span<uint8_t> &data);

void handleClient(Client &client);
} // namespace minecraft::protocol
