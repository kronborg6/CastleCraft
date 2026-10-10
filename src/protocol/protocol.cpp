#include "protocol.hpp"
#include "client.hpp"
#include "handlers/handshake.hpp"
#include "handlers/login.hpp"
#include "handlers/status.hpp"
#include <arpa/inet.h>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <optional>
#include <stdexcept>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

namespace minecraft::protocol {

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

  std::optional<State> toState(int32_t value) {
    switch (value) {
    case 0:
      return State::Handshake;
    case 1:
      return State::Status;
    case 2:
      return State::Login;
    case 3:
      return State::Configuration;
    case 4:
      return State::Play;
    default:
      return std::nullopt;
    }
  }

  void handleClinet(Client& client) {

    switch (client.getState()) {
    case State::Handshake:
      handlers::handleHandshake(client);
      break;
    case State::Status:
      handlers::handleStatus(client);
      break;
    case State::Login:
      // handleLogin(client);
      break;
    case State::Configuration:
      // handleConfguration(client);
      break;
    case State::Play:
      // handlePlay(client);
      break;
    }
  }
} // namespace minecraft::protocol
