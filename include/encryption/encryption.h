#ifndef ENCRYPTION_H
#define ENCRYPTION_H

#include <vector>
#include <cstdint>
#include <iostream>
#include "encryption/key_derivation.h"
#include "encryption/data_conversion.h"
#include "encryption/utils.h"
#include "CliConfig.h"

namespace Encryption {
    /**
     * @brief Prepares text for embedding in an image.
     * 
     * This function retrieves and processes the input text from the CLI configuration,
     * encrypts it if necessary, and returns it as a binary vector ready for steganographic embedding.
     * 
     * @param config The CLI configuration containing user-specified parameters.
     * @return std::vector<uint8_t> A vector containing the processed text, ready for embedding.
     */
    std::vector<uint8_t> getReadyToEmbedText(CliConfig& config);
} // namespace Encryption

namespace {
    /**
     * @brief Encrypts data using AES-256-CBC.
     * 
     * This function encrypts the given plaintext using AES-256-CBC mode.
     * It generates a random IV (16 bytes), which is prepended to the encrypted data.
     * 
     * @param plaintext A vector containing the plaintext data to be encrypted.
     * @param key The binary encryption key (32 bytes).
     * @return std::vector<uint8_t> A vector containing the IV (16 bytes) followed by the encrypted data.
     * @throws std::runtime_error If the key size is incorrect or an error occurs during encryption.
     */
    std::vector<uint8_t> encryptData(const std::vector<uint8_t>& plaintext, const std::vector<uint8_t>& key);
}

#endif // ENCRYPTION_H
