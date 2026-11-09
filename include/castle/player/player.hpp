#pragma once
#include <cstdint>
#include <cstdlib>
#include <iostream>

struct Teleport {
    std::int32_t id = -1;
    bool used = false;
};

namespace castle::player {
  class Player {
    public:
      Player(std::int32_t id) : _entityId(id) {};
      Player(Player&&) = default;
      Player(const Player&) = default;
      Player& operator=(Player&&) = default;
      Player& operator=(const Player&) = default;
      ~Player() = default;

      std::int32_t entityId() const {
        return _entityId;
      }

      std::int32_t newTeleport() {
        if (_teleport.used == false && _teleport.id != -1) {
          return -1;
        }
        std::int32_t randomNUm = static_cast<std::int32_t>(rand() % 2500000);

        _teleport = Teleport{.id = randomNUm, .used = false};
        return randomNUm;
      }

      std::int32_t teleportId() const {
        return _teleport.id;
      }

      bool checkTeleport(std::int32_t id) {
        if (_teleport.id == id) {
          _teleport.used = true;
          return true;
        }
        return false;
      }

    private:
      std::int32_t _entityId;
      Teleport _teleport;
  };

} // namespace castle::player
