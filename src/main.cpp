#include "client.hpp"
#include "protocol/protocol.hpp"
#include <arpa/inet.h>
#include <cerrno>
#include <cstdint>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <netinet/in.h>
#include <stdexcept>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>
#define PROXCY false

#include <nbt/nbt.hpp>

// void disconect(std::string username, int client) {
//   const std::string json = R"({"text":"hello )" + username + R"(, login
//   is not implemented yet!"})";
//
//   std::vector<uint8_t> response;
//
//   writeString(json, response);
//
//   sendPacket(client, 0x00, response);
// }
//
//

void printHex(const std::vector<uint8_t>& data) {
  for (uint8_t byte : data) {
    std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte) << ' ';
  }

  std::cout << std::dec << '\n';
}
int main() {

  std::vector<uint8_t> data;

  // castle::nbt::ToTagCompound("hello world", data);
  // minecraft::nbt::ToTagString("hello", "kronborg", data);
  // minecraft::nbt::ToTagCompound("stats", data);
  // minecraft::nbt::ToTagFloat("health", 100.01f, data);
  // minecraft::nbt::ToTagInt("lvl", 16, data);
  // minecraft::nbt::ToTagInt("xp", 512, data);
  // minecraft::nbt::ToEndTag(data);
  // minecraft::nbt::ToEndTag(data);

  printHex(data);

  int server = socket(AF_INET, SOCK_STREAM, 0);

  if (server < 0) {
    std::cerr << "Could not create socket\n";
    return 1;
  }

  int reuse = 1;

  setsockopt(server, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));

  sockaddr_in address{};

  uint16_t port = 25565;

  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(port);

  if (bind(server, reinterpret_cast<sockaddr*>(&address), sizeof(address)) < 0) {
    if (errno == EADDRINUSE) {
      port = 25566;
      address.sin_port = htons(port);

      if (bind(server, reinterpret_cast<sockaddr*>(&address), sizeof(address)) < 0) {
        std::cerr << "failed to bind to port " << port << ": " << std::strerror(errno) << '\n';
        return 1;
      }
    } else {

      std::cerr << "bind failed: " << std::strerror(errno) << '\n';
      return 1;
    }
  }

  if (listen(server, 16) < 0) {
    std::cerr << "Listen failed\n";
    return 1;
  }
  std::cout << "Minecraft server listening on port " << port << "\n";

  while (true) {
    int socket = accept(server, nullptr, nullptr);

    if (socket < 0) {
      continue;
    }
    Client client(socket);

    try {
      minecraft::protocol::handleClient(client);
    } catch (const std::exception& error) {
      std::cerr << "Client error: " << error.what() << "\n";
    }
  }

  close(server);
}
