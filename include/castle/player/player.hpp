#pragma once
#include "castle/player/postion.hpp"
#include "castle/world/chunkPostion.hpp"
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <unordered_map>

struct Teleport {
    std::int32_t id = -1;
    bool used = false;
};

namespace castle::player {

  constexpr std::int32_t VIEW = 10;
  class Player {
    public:
      Player(std::int32_t id) : _entityId(id) {};

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

      bool updatePostion(const Position& postion, UpdateType type) {
        auto oldChunk = _chunkPosition;
        switch (type) {
        case UpdateType::Position:
          _position.x = postion.x;
          _position.y = postion.y;
          _position.z = postion.z;
          break;
        case UpdateType::Rotation:
          _position.pitch = postion.pitch;
          _position.yaw = postion.yaw;
          break;
        case UpdateType::PositionAndRotation:
          _position = postion;
          break;
        }
        _position.flags = postion.flags;

        if (type == UpdateType::Rotation) {
          return false;
        }
        _chunkPosition = world::ChunkPosition{
            .x = static_cast<std::int32_t>(std::floor(_position.x / 16.0)),
            .z = static_cast<std::int32_t>(std::floor(_position.z / 16.0)),
        };

        if (_chunkPosition != oldChunk) {
          std::cout << "Player entered chunk: (" << _chunkPosition.x << ";" << _chunkPosition.z
                    << ")\n";
        }

        return _chunkPosition != oldChunk;
      }

      world::ChunkPosition getChunkPostion() const {
        return _chunkPosition;
      }
      Position getPostion() const {
        return _position;
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
      castle::player::Position _position;
      std::uint8_t _moment = 0x00;
      world::ChunkPosition _chunkPosition;
  };

} // namespace castle::player
