# CastleCraft

CastleCraft is an experimental Minecraft server written in C++. It is a personal learning project focused on C++, project design, TCP networking, and implementing an existing protocol.

## Current status

The project is in early development. It can accept connections, handle the handshake, respond to status and ping requests, and perform an early login flow. The protocol implementation is incomplete and is not ready for production use.

## Build and run

Requirements: CMake 4.2 or newer and a C++23 compiler.

```sh
cmake -S . -B build
cmake --build build
./build/minecraft-server
```

The server listens on port `25565`, or `25566` if that port is already in use.

## Goals

- Improve my C++ and TCP networking skills.
- Learn how to work with a protocol I did not design.
- Support multiple Minecraft protocol versions.
- Experiment with a proxy in C++ or Rust for logging.
- Eventually create a better map generator.

This is a personal learning project and is not intended for production use.

> README written with assistance from ChatGPT.

