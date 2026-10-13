#include <cstddef>
#include <cstdint>
#include <nbt/nbt.hpp>
#include <stdexcept>
#include <string>
#include <vector>

namespace minecraft::nbt {

void ToEndCompound(std::vector<uint8_t> &data) { data.push_back(0x00); }
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
void ToTagCompound(const std::string &name, std::vector<std::uint8_t> &data) {
	const std::size_t length = name.length();

	if (length > UINT16_MAX) {
		throw std::runtime_error("NBT tag name is too long");
	}

	const uint16_t rawLength = static_cast<uint16_t>(length);

	data.push_back(0x0a);

	data.push_back(static_cast<std::uint8_t>((rawLength >> 8) & 0xFF));
	data.push_back(static_cast<std::uint8_t>(rawLength & 0xFF));

	data.insert(data.end(), name.begin(), name.end());

	ToEndCompound(data);
}

void ToTagString(const std::string &name, const std::string &value,
                 std::vector<std::uint8_t> &data) {

	const std::size_t length = name.length();
	const std::size_t valueLength = value.length();

	if (length > UINT16_MAX || valueLength > UINT16_MAX) {
		throw std::runtime_error("NBT tag name is too long");
	}

	const uint16_t rawLength = static_cast<std::uint16_t>(length);
	const uint16_t rawValueLength = static_cast<std::uint16_t>(valueLength);

	data.push_back(0x08);

	data.push_back(static_cast<uint8_t>((rawLength >> 8) & 0xFF));
	data.push_back(static_cast<uint8_t>(rawLength & 0xFF));
	data.insert(data.end(), name.begin(), name.end());

	data.push_back(static_cast<uint8_t>((rawValueLength >> 8) & 0xFF));
	data.push_back(static_cast<uint8_t>(rawValueLength & 0xFF));
	data.insert(data.end(), value.begin(), value.end());
}
} // namespace minecraft::nbt
