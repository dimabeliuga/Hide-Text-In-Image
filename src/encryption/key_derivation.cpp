#include "encryption/key_derivation.h"
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <stdexcept>
#include "external/logger.h"

namespace KeyDerivation {

std::vector<uint8_t> deriveKey(const std::string& passphrase, 
                               const std::vector<uint8_t>& salt, 
                               int iterations, 
                               size_t keyLength) {
    std::vector<uint8_t> key(keyLength);
    const EVP_MD* digest = EVP_sha256(); // Используем HMAC-SHA256

    // Применяем PBKDF2 для вывода ключа
    int ret = PKCS5_PBKDF2_HMAC(passphrase.c_str(),
                                static_cast<int>(passphrase.length()),
                                salt.data(),
                                static_cast<int>(salt.size()),
                                iterations,
                                digest,
                                static_cast<int>(keyLength),
                                key.data());
    if (ret != 1) {
        LOG_ERROR("Key derivation failed using PBKDF2");
        exit(EXIT_FAILURE);
    }

    LOG_INFO("Key derivation went succesfully");
    return key;
}

std::vector<uint8_t> generateSalt(size_t saltLength) {
    std::vector<uint8_t> salt(saltLength);
    if (RAND_bytes(salt.data(), static_cast<int>(saltLength)) != 1) {
        LOG_ERROR("Failed to generate random salt");
        exit(EXIT_FAILURE);
    }
    LOG_INFO("Salt was generated");
    return salt;
}

} // namespace KeyDerivation
