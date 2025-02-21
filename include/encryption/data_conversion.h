#ifndef DATA_CONVERSION_H
#define DATA_CONVERSION_H

#include <vector>
#include <cstdint>
#include <iostream>

namespace DataConversion {
    // Constants
    constexpr size_t HEADER_SIZE = 4;
    constexpr size_t SALT_SIZE = 16;
    constexpr int KDF_ITERATIONS = 10'000;

    // Function declarations (remove implementations)
    std::vector<uint8_t> stringToBytes(const std::string& str);
    std::vector<uint8_t> uint32ToBytes(uint32_t value);
    uint32_t bytesToUint32(const std::vector<uint8_t>& bytes);
}

#endif