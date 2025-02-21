#include "encryption/utils.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <stdexcept>
#include <openssl/hmac.h>
#include <openssl/rand.h>

namespace Utils {

    std::vector<uint8_t> computeHMAC(const std::vector<uint8_t>& data, const std::vector<uint8_t>& key) {
        unsigned int len = EVP_MAX_MD_SIZE;
        unsigned char hmacResult[EVP_MAX_MD_SIZE];

        // Вычисляем HMAC с использованием алгоритма HMAC-SHA256
        if (HMAC(EVP_sha256(),
                 key.data(), static_cast<int>(key.size()),
                 data.data(), data.size(),
                 hmacResult, &len) == nullptr) {
            LOG_ERROR("HMAC calculation failed");
            exit(EXIT_FAILURE);
        }

        return std::vector<uint8_t>(hmacResult, hmacResult + len);
    }

    std::string bytesToHex(const std::vector<uint8_t>& data) {
        std::ostringstream oss;
        for (auto byte : data) {
            oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte);
        }
        return oss.str();
    }

    std::vector<uint8_t> hexToBytes(const std::string& hex) {
        if (hex.size() % 2 != 0) {
            LOG_ERROR("Hex string has an invalid length");
            exit(EXIT_FAILURE);
        }
        std::vector<uint8_t> bytes;
        bytes.reserve(hex.size() / 2);
        for (size_t i = 0; i < hex.size(); i += 2) {
            std::string byteString = hex.substr(i, 2);
            uint8_t byte = static_cast<uint8_t>(std::stoi(byteString, nullptr, 16));
            bytes.push_back(byte);
        }
        return bytes;
    }

    std::vector<uint8_t> getRandomBytes(size_t length) {
        std::vector<uint8_t> randomData(length);
        if (RAND_bytes(randomData.data(), static_cast<int>(length)) != 1) {
            LOG_ERROR("Failed to generate random bytes");
            exit(EXIT_FAILURE);
        }
        return randomData;
    }

} // namespace Utils
