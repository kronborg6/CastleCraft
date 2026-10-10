#include <arpa/inet.h>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

int32_t readVarInt(int socket) {
  int32_t value = 0;
  int position = 0;

  while (true) {
    uint8_t byte;

    if (recv(socket, &byte, 1, MSG_WAITALL) != 1) {
      throw std::runtime_error("Failed to read VarInt");
    }

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

void writeVarInt(int32_t value, std::vector<uint8_t>& output) {
  while (true) {
    if ((value & ~0x7F) == 0) {
      output.push_back(static_cast<uint8_t>(value));
      return;
    }

    output.push_back(static_cast<uint8_t>((value & 0x7F) | 0x80));

    value = static_cast<uint32_t>(value) >> 7;
  }
}

std::string readString(int socket) {
  int32_t length = readVarInt(socket);

  std::string result(length, '\0');

  if (recv(socket, result.data(), length, MSG_WAITALL) != length) {
    throw std::runtime_error("Failed to read string");
  }

  return result;
}

void writeString(const std::string& text, std::vector<uint8_t>& output) {
  writeVarInt(static_cast<int32_t>(text.size()), output);

  output.insert(output.end(), text.begin(), text.end());
}
