#pragma once

#include <string_view>

namespace castle::protocol {
  enum class State {
    Handshake,
    Status,
    Login,
    Configuration,
    Play
  };

  inline std::string_view stateToString(State state) {
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
} // namespace castle::protocol
