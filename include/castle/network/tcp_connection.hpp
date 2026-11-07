#pragma once

#include <cstddef>
#include <cstdint>
#include <unistd.h>

namespace csatle::network {

class TcpConnection {
public:
  explicit TcpConnection(int socketId) : _socket(socketId) {};

  TcpConnection(const TcpConnection &) = delete;
  TcpConnection &operator=(const TcpConnection &) = delete;

  TcpConnection(TcpConnection &&) = delete;
  TcpConnection &operator=(TcpConnection &&) = delete;

  ~TcpConnection() {
    if (_socket >= 0) {
      close(_socket);
    }
  };

  void readAll(void *buffer, std::size_t size);
  void writeAll(const void *buffer, std::size_t size);

  std::uint8_t readByte();

private:
  int _socket;
};
} // namespace csatle::network
