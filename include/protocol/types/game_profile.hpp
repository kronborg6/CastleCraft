#pragma once

#include <array>
#include <cstdint>
#include <optional>
#include <string>
#include <vector>

struct GameProfileProperty {
	std::string name;
	std::string value;
	std::optional<std::string> signature;
};

struct GameProfile {
	std::array<uint8_t, 16> uuid;
	std::string username;
	std::vector<GameProfileProperty> properties;
};
