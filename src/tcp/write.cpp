#include <arpa/inet.h>
#include <cstdint>
#include <cstring>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

void sendPacket(int socket, int32_t packetId, const std::vector<uint8_t>& data) {
  std::vector<uint8_t> packet;

  writeVarInt(packetId, packet);

  packet.insert(packet.end(), data.begin(), data.end());

  std::vector<uint8_t> framed;

  writeVarInt(static_cast<int32_t>(packet.size()), framed);

  framed.insert(framed.end(), packet.begin(), packet.end());

  send(socket, framed.data(), framed.size(), 0);
}
