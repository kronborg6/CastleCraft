#include "castle/network/tcp_connection.hpp"
#include <cstddef>
#include <cstdint>
#include <mutex>
#include <stdexcept>
#include <sys/socket.h>
#include <unistd.h>

void castle::network::TcpConnection::readAll(void* buffer, std::size_t size) {
  uint8_t* byte = static_cast<uint8_t*>(buffer);

  size_t totalRead = 0;

  while (totalRead < size) {
    const ssize_t result = recv(_socket, byte + totalRead, size - totalRead, 0);

    if (result == 0) {
      throw std::runtime_error("Client disconnected");
    }

    if (result < 0) {
      throw std::runtime_error("socket read failed");
    }
    totalRead += static_cast<size_t>(result);
  }
}

void castle::network::TcpConnection::writeAll(const void* buffer, size_t size) {
  std::lock_guard<std::mutex> lock(_socketMutex);
  const uint8_t* bytes = static_cast<const uint8_t*>(buffer);

  size_t totalSent = 0;

  while (totalSent < size) {

    const ssize_t result = send(_socket, bytes + totalSent, size - totalSent, 0);

    if (result <= 0) {
      throw std::runtime_error("Socket write falied");
    }
    totalSent += static_cast<size_t>(result);
  }
}

uint8_t castle::network::TcpConnection::readByte() {
  uint8_t byte;
  readAll(&byte, 1);
  return byte;
}
