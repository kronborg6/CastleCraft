#include "castle/handlers/play/keepALive.hpp"
#include "castle/handlers/play/play.hpp"

namespace castle::handlers::play {
  void keepALive(PlayPacketReader packet, Client& client) {
    auto now = std::chrono::steady_clock::now();

    auto duration = now.time_since_epoch();

    std::int64_t milliseconds =
        std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();

    auto value = packet.getInt<std::int64_t>();
    if (value != client.lastPing) {
      client.disconected();
      // send a boardcast that user have lost connection
    }
    // use take value and sumtack milliseconds to get reponse time and use as ping
  }
} // namespace castle::handlers::play
