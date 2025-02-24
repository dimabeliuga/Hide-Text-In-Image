#ifndef DATA_CONVERSION_H
#define DATA_CONVERSION_H

#include <vector>
#include <cstdint>
#include <iostream>

namespace DataConversion {
    /**
     * @brief Size of the header in bytes.
     */
    constexpr size_t HEADER_SIZE = 4;

    /**
     * @brief Size of the salt in bytes.
     */
    constexpr size_t SALT_SIZE = 16;

    /**
     * @brief Number of iterations for the Key Derivation Function (KDF).
     */
    constexpr int KDF_ITERATIONS = 10'000;

    /**
     * @brief Converts a string to a vector of bytes.
     * @param str The string to convert.
     * @return A vector containing the bytes of the string.
     */
    std::vector<uint8_t> stringToBytes(const std::string& str);

    /**
     * @brief Converts a uint32_t value to a vector of bytes.
     * @param value The uint32_t value to convert.
     * @return A vector containing the bytes of the uint32_t value.
     */
    std::vector<uint8_t> uint32ToBytes(uint32_t value);

    /**
     * @brief Converts a vector of bytes to a uint32_t value.
     * @param bytes The vector of bytes to convert.
     * @return The uint32_t value obtained from the bytes.
     */
    uint32_t bytesToUint32(const std::vector<uint8_t>& bytes);
}

#endif // DATA_CONVERSION_H
