#include "client.hpp"
#include "protocol/protocol.hpp"
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
//   const std::string json = R"({"text":"hello )" + username + R"(, login is
//   not implemented yet!"})";
//
//   std::vector<uint8_t> response;
//
//   writeString(json, response);
//
//   sendPacket(client, 0x00, response);
// }
//

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

	if (bind(server, reinterpret_cast<sockaddr *>(&address), sizeof(address)) <
	    0) {

		std::cerr << "Bind failed\n";
		return 1;
	}

	if (listen(server, 16) < 0) {
		std::cerr << "Listen failed\n";
		return 1;
	}

	std::cout << "Minecraft server listening on port 25565\n";

	while (true) {
		int socket = accept(server, nullptr, nullptr);

		if (socket < 0) {
			continue;
		}
		Client client(socket);

		try {
			minecraft::protocol::handleClient(client);
		} catch (const std::exception &error) {
			std::cerr << "Client error: " << error.what() << "\n";
		}
	}

	close(server);
}
