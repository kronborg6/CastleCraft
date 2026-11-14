#pragma once

#include "castle/network/tcp_connection.hpp"
namespace castle::entries {
  void cowEntrie(castle::network::TcpConnection& connection);

  void cowSoundEntrie(castle::network::TcpConnection& connection);
  void catEntrie(castle::network::TcpConnection& connection);
  void catSoundEntrie(castle::network::TcpConnection& connection);
  void chickenEntrie(castle::network::TcpConnection& connection);
  void chickenSoundEntrie(castle::network::TcpConnection& connection);
  void frogEntrie(castle::network::TcpConnection& connection);
  void paintingEntrie(castle::network::TcpConnection& connection);
  void pigEntrie(castle::network::TcpConnection& connection);
  void pigSoundEntrie(castle::network::TcpConnection& connection);
  void wolfEntrie(castle::network::TcpConnection& connection);
  void wolfSoundEntrie(castle::network::TcpConnection& connection);
  void zombieEntrie(castle::network::TcpConnection& connection);
} // namespace castle::entries
