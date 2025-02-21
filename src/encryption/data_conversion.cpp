#include "encryption/data_conversion.h"
#include "external/logger.h"

namespace DataConversion {
    std::vector<uint8_t> stringToBytes(const std::string& str) {
        return std::vector<uint8_t>(str.begin(), str.end());
    }

    std::vector<uint8_t> uint32ToBytes(uint32_t value) {
        std::vector<uint8_t> bytes(4);
        bytes[0] = (value >> 24) & 0xFF;
        bytes[1] = (value >> 16) & 0xFF;
        bytes[2] = (value >> 8)  & 0xFF;
        bytes[3] = value & 0xFF;
        return bytes;
    }

    uint32_t bytesToUint32(const std::vector<uint8_t>& bytes) {
        if (bytes.size() != 4) {
            LOG_ERROR("Unright size of a head for uint32_t");
            exit(EXIT_FAILURE);
        }
        uint32_t value = 0;
        value |= (static_cast<uint32_t>(bytes[0]) << 24);
        value |= (static_cast<uint32_t>(bytes[1]) << 16);
        value |= (static_cast<uint32_t>(bytes[2]) << 8);
        value |= (static_cast<uint32_t>(bytes[3]));
        return value;
    }
}