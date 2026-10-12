#include <cstddef>
#include <cstdint>
#include <nbt/nbt.hpp>

namespace minecraft::nbt {
void ToTagShort(const std::string &name, int16_t value,
                std::vector<std::uint8_t> &data) {
	const size_t length = name.length();

	data.push_back(static_cast<uint8_t>(0x02));

	const uint16_t rawLength = static_cast<uint16_t>(length);
	const uint16_t rawValue = static_cast<uint16_t>(value);

	data.push_back(static_cast<uint8_t>((length >> 8) & 0xFF));
	data.push_back(static_cast<uint8_t>(length & 0xFF));
	data.insert(data.end(), name.begin(), name.end());
	data.push_back(static_cast<uint8_t>((value >> 8) & 0xFF));
	data.push_back(static_cast<uint8_t>(value & 0xFF));
}
void ToTagInt(const std::string &name, int32_t value,
              std::vector<std::uint8_t> &data) {
	const size_t length = name.length();

	data.push_back(static_cast<uint8_t>(0x03));

	const uint16_t rawLength = static_cast<uint16_t>(length);
	const uint32_t rawValue = static_cast<uint32_t>(value);

	data.push_back(static_cast<uint8_t>((rawLength >> 8) & 0xFF));
	data.push_back(static_cast<uint8_t>(rawLength & 0xFF));
	data.insert(data.end(), name.begin(), name.end());
	data.push_back(static_cast<uint8_t>((rawValue >> 24) & 0xFF));
	data.push_back(static_cast<uint8_t>((rawValue >> 16) & 0xFF));
	data.push_back(static_cast<uint8_t>((rawValue >> 8) & 0xFF));
	data.push_back(static_cast<uint8_t>(rawValue & 0xFF));
}
void ToTagLong(const std::string &name, int64_t value,
               std::vector<std::uint8_t> &data) {
	const size_t length = name.length();

	data.push_back(static_cast<uint8_t>(0x04));

	const uint16_t rawLength = static_cast<uint16_t>(length);
	const uint64_t rawValue = static_cast<uint64_t>(value);

	data.push_back(static_cast<uint8_t>((rawLength >> 8) & 0xFF));
	data.push_back(static_cast<uint8_t>(rawLength & 0xFF));
	data.insert(data.end(), name.begin(), name.end());
	data.push_back(static_cast<uint8_t>((rawValue >> 56) & 0xFF));
	data.push_back(static_cast<uint8_t>((rawValue >> 48) & 0xFF));
	data.push_back(static_cast<uint8_t>((rawValue >> 40) & 0xFF));
	data.push_back(static_cast<uint8_t>((rawValue >> 32) & 0xFF));
	data.push_back(static_cast<uint8_t>((rawValue >> 24) & 0xFF));
	data.push_back(static_cast<uint8_t>((rawValue >> 16) & 0xFF));
	data.push_back(static_cast<uint8_t>((rawValue >> 8) & 0xFF));
	data.push_back(static_cast<uint8_t>(rawValue & 0xFF));
}
} // namespace minecraft::nbt
