#ifndef DECRYPTION_H
#define DECRYPTION_H

#include "encryption/key_derivation.h"
#include "encryption/utils.h"
#include "encryption/data_conversion.h"
#include "external/logger.h"
#include "image_handler.h"
#include "CliConfig.h"
#include "stegano.h"

#include <string>

namespace Decryption {
    /**
     * @brief Extracts and decrypts a hidden message from an image.
     * 
     * This function retrieves the encrypted data hidden within the image
     * and decrypts it using the provided steganographic key.
     * 
     * @param config The CLI configuration containing user-specified parameters.
     * @param image The image object from which the hidden message will be extracted.
     * @param steganoKey The key used for extracting and decrypting the hidden message.
     * @return std::string The decrypted message.
     */
    std::string getDecryptedMessage(const CliConfig& config, ImageHandler::Image& image, std::vector<uint8_t>& steganoKey);
}

namespace {
    /**
     * @brief Decrypts data encrypted using AES-256-CBC.
     * 
     * This function expects a vector where the first 16 bytes represent the IV (Initialization Vector),
     * followed by the encrypted data. After decryption, the original plaintext is returned.
     * 
     * @param ciphertext A vector containing the IV (16 bytes) followed by the encrypted data.
     * @param key The binary decryption key (32 bytes).
     * @return std::vector<uint8_t> A vector containing the decrypted plaintext.
     * @throws std::runtime_error If the key size is incorrect, the data is insufficient, or an error occurs during decryption.
     */
    std::vector<uint8_t> decryptData(const std::vector<uint8_t>& ciphertext, const std::vector<uint8_t>& key);
}

#endif // DECRYPTION_H
