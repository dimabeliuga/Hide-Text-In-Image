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


namespace Decryption{
    // dectypt function
    std::string getDecryptedMessage(const CliConfig& config, ImageHandler::Image& image, std::vector<uint8_t>& steganoKey);

}

namespace {
        
    /**
     * @brief Дешифрует данные, зашифрованные с использованием AES-256-CBC.
     *
     * Функция принимает вектор, в котором первые 16 байт — это IV, а остальные — зашифрованные данные.
     * После расшифрования возвращается исходный текст.
     *
     * @param ciphertext Вектор, содержащий IV (16 байт) + зашифрованные данные.
     * @param key Бинарный ключ (32 байта).
     * @return std::vector<uint8_t> Вектор с расшифрованными данными.
     * @throws std::runtime_error Если размер ключа неверен, данных недостаточно или при ошибке расшифрования.
     */
    std::vector<uint8_t> decryptData(const std::vector<uint8_t>& ciphertext, const std::vector<uint8_t>& key);
}
#endif