#include "castle/handlers/play/block.hpp"
#include "castle/client.hpp"
#include "castle/handlers/play/play.hpp"
#include "castle/server/server.hpp"
#include "castleProtocol/types/postion.hpp"
#include <cstdint>
#include <stdexcept>
namespace castle::handlers::play {

  void updatePostionFromFace(std::int32_t face, type::Postion& postion) {
    switch (face) {
    case 0:
      postion.y--;
      break;
    case 1:
      postion.y++;
      break;
    case 2:
      postion.z--;
      break;
    case 3:
      postion.z++;
      break;
    case 4:
      postion.x--;
      break;
    case 5:
      postion.x++;
      break;
    default:
      throw std::runtime_error("not posibel face");
      break;
    }
  }

  void useBlockOn(server::Server& server, Client& client, PlayPacketReader packet) {
    auto hand = packet.varInt();
    auto postion = packet.getPosition();
    auto face = packet.varInt();
    auto cursorX = packet.getFloat();
    auto cursorY = packet.getFloat();
    auto cursorZ = packet.getFloat();
    auto insideBlock = packet.byte();
    auto worldBorderHit = packet.byte();
    auto sequence = packet.varInt();

    std::cout << "Hand: " << hand << '\n'
              << "Position: " << postion.x << ", " << postion.y << ", " << postion.z << '\n'
              << "Face: " << face << '\n'
              << "Cursor X: " << cursorX << '\n'
              << "Cursor Y: " << cursorY << '\n'
              << "Cursor Z: " << cursorZ << '\n'
              << "Inside Block: " << static_cast<bool>(insideBlock) << '\n'
              << "World Border Hit: " << static_cast<bool>(worldBorderHit) << '\n'
              << "Sequence: " << sequence << '\n';

    updatePostionFromFace(face, postion);

    auto inv = client.getPlayer().getInventory();
    auto itemUsed = inv.getHeldItem();

    if (itemUsed != nullptr) {
      server.setBlock(postion, 9);
      std::cout << "ItemID: " << itemUsed->itemId << std::endl;
    }
  }
} // namespace castle::handlers::play
