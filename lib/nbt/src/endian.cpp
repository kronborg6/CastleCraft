#include <cstdint>
#include <vector>

namespace endian {
template <typename T> void writeBigEndian(T value, std::vector<uint8_t> &data) {
	static_assert(std::is_unsigned_v<T>);

	for (int shift = static_cast<int>(sizeof(T) * 8) - 8; shift >= 0;
	     shift -= 8) {
		data.push_back(static_cast<std::uint8_t>((value >> shift) & 0xFF));
	}
}
template <typename T>
void writeLittleEndian(T value, std::vector<uint8_t> &data) {
	static_assert(std::is_unsigned_v<T>);

	for (int shift = 0; shift < static_cast<int>(sizeof(T) * 8); shift += 8) {
		data.push_back(static_cast<std::uint8_t>((value >> shift) & 0xFF));
	}
}

uint32_t floatToUint32(float value) {

	uint32_t reslut = static_cast<uint32_t>(value);
	return reslut;
}
uint64_t doubleToUint64(double value) {
	uint64_t reslut = static_cast<uint64_t>(value);
	return reslut;
}
} // namespace endian
