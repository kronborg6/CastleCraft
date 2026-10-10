#pragma once

#include <cstdint>
#include <string>
#include <vector>
namespace minecraft::protocol {
  enum class State {
    Handshake,
    Status,
    Login,
    Configuration,
    Play
  };

  std::string_view stateToString(State state) {
    switch (state) {
    case State::Handshake:
      return "Handshake";
    case State::Status:
      return "Status";
    case State::Login:
      return "Login";
    case State::Configuration:
      return "Configuration";
    case State::Play:
      return "Play";
    }

    return "Unknown";
  }
  int32_t readVarInt(int socket);
  void writeVarInt(int32_t value, std::vector<uint8_t>& output);
  std::string readString(int socket);
  void writeString(const std::string& text, std::vector<uint8_t>& output);

  void handleClinet(int socketId);
} // namespace minecraft::protocol
