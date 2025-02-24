#ifndef KEY_DERIVATION_H
#define KEY_DERIVATION_H

#include <vector>
#include <cstdint>
#include <string>

namespace KeyDerivation {

    /**
     * @brief Derives a binary key from a string passphrase using PBKDF2 with HMAC-SHA256.
     *
     * @param passphrase The string key entered by the user.
     * @param salt The salt to enhance the strength of the derived key.
     * @param iterations The number of iterations (recommended at least 10000).
     * @param keyLength The desired length of the binary key in bytes.
     * @return std::vector<uint8_t> The derived binary key.
     * @throws std::runtime_error If key derivation fails.
     */
    std::vector<uint8_t> deriveKey(const std::string& passphrase, 
                                   const std::vector<uint8_t>& salt, 
                                   int iterations, 
                                   size_t keyLength);

    /**
     * @brief Generates a cryptographically strong random salt of the specified length.
     *
     * @param saltLength The length of the salt in bytes.
     * @return std::vector<uint8_t> The generated salt.
     * @throws std::runtime_error If salt generation fails.
     */
    std::vector<uint8_t> generateSalt(size_t saltLength);

} // namespace KeyDerivation

#endif // KEY_DERIVATION_H
