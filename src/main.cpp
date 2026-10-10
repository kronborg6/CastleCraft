#include <arpa/inet.h>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

// void disconect(std::string username, int client) {
//   const std::string json = R"({"text":"hello )" + username + R"(, login is not implemented
//   yet!"})";
//
//   std::vector<uint8_t> response;
//
//   writeString(json, response);
//
//   sendPacket(client, 0x00, response);
// }
//
// void handleClient(int client) {
//   //
//   // HANDSHAKE
//   //
//
//   int32_t packetLength = readVarInt(client);
//   (void)packetLength;
//
//   int32_t packetId = readVarInt(client);
//
//   if (packetId != 0) {
//     return;
//   }
//
//   int32_t protocolVersion = readVarInt(client);
//
//   std::string address = readString(client);
//
//   uint8_t portBytes[2];
//
//   recv(client, portBytes, 2, MSG_WAITALL);
//
//   uint16_t port = (static_cast<uint16_t>(portBytes[0]) << 8) | portBytes[1];
//
//   int32_t nextState = readVarInt(client);
//
//   std::cout << "Protocol: " << protocolVersion << "\n";
//
//   std::cout << "Next State: " << nextState << "\n";
//
//   std::cout << "Address: " << address << "\n";
//
//   std::cout << "Port: " << port << "\n";
//
//   if (nextState != 1) {
//
//     int32_t _packet_length = readVarInt(client);
//
//     int32_t packetId = readVarInt(client);
//     std::string username = readString(client);
//     std::cout << "player: " << username << " trying to login\n";
//
//     disconect(username, client);
//
//     return;
//   }
//
//   //
//   // STATUS REQUEST
//   //
//
//   readVarInt(client);
//
//   int32_t statusPacket = readVarInt(client);
//
//   if (statusPacket != 0) {
//     return;
//   }
//
//   const std::string json = R"({
//         "version": {
//             "name": "My C++ Server",
//             "protocol": 777
//         },
//         "players": {
//             "max": 69,
//             "online": 420,
//             "sample": [
//       {
//         "name": "kronborg",
//         "id": "82f59091-e5e2-4638-b6d8-4108c7851b95"
//       }
//             ]
//         },
//         "description": {
//             "text": "Hello from C++!"
//         }
//     })";
//
//   std::vector<uint8_t> response;
//
//   writeString(json, response);
//
//   sendPacket(client, 0x00, response);
//
//   //
//   // PING
//   //
//
//   readVarInt(client);
//
//   int32_t pingPacket = readVarInt(client);
//
//   if (pingPacket == 0x01) {
//     std::vector<uint8_t> pingPayload(8);
//
//     recv(client, pingPayload.data(), 8, MSG_WAITALL);
//
//     sendPacket(client, 0x01, pingPayload);
//   }
// }

int main() {
  int server = socket(AF_INET, SOCK_STREAM, 0);

  if (server < 0) {
    std::cerr << "Could not create socket\n";
    return 1;
  }

  int reuse = 1;

  setsockopt(server, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));

  sockaddr_in address{};

  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(25565);

  if (bind(server, reinterpret_cast<sockaddr*>(&address), sizeof(address)) < 0) {

    std::cerr << "Bind failed\n";
    return 1;
  }

  if (listen(server, 16) < 0) {
    std::cerr << "Listen failed\n";
    return 1;
  }

  std::cout << "Minecraft server listening on port 25565\n";

  while (true) {
    int client = accept(server, nullptr, nullptr);

    if (client < 0) {
      continue;
    }

    try {
      // handleClient(client);
    } catch (const std::exception& error) {
      std::cerr << "Client error: " << error.what() << "\n";
    }

    close(client);
  }

  close(server);
}
