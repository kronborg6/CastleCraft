#pragma once
#include "tcp.hpp"

namespace castle::entries {
  void cowEntrie(TcpConnection& connection);

  void cowSoundEntrie(TcpConnection& connection);
  void catEntrie(TcpConnection& connection);
  void catSoundEntrie(TcpConnection& connection);
  void chickenEntrie(TcpConnection& connection);
  void chickenSoundEntrie(TcpConnection& connection);
  void frogEntrie(TcpConnection& connection);
  void paintingEntrie(TcpConnection& connection);
  void pigEntrie(TcpConnection& connection);
  void pigSoundEntrie(TcpConnection& connection);
  void wolfEntrie(TcpConnection& connection);
  void wolfSoundEntrie(TcpConnection& connection);
  void zombieEntrie(TcpConnection& connection);
} // namespace castle::entries
