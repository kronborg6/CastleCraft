#include "castle/client.hpp"
#include "castle/handlers/play/play.hpp"
#include "castle/server/server.hpp"
namespace castle::handlers::play {
  void useBlockOn(server::Server& server, Client& client, PlayPacketReader packet);
}
