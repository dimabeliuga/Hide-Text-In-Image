#ifndef ENCRYPTION_H
#define ENCRYPTION_H

#include <vector>
#include <cstdint>
#include <iostream>
#include "encryption/key_derivation.h"
#include "encryption/data_conversion.h"
#include "encryption/utils.h"
#include "CliConfig.h"

// Все функции находятся в пространстве имён Encryption.
namespace Encryption {

    std::vector<uint8_t> getReadyToEmbedText(CliConfig& config);

} // namespace Encryption

namespace {
    /**
     * @brief Шифрует данные с использованием AES-256-CBC.
     *
     * Функция принимает исходный текст (plaintext) и бинарный ключ (key) размером 32 байта.
     * В процессе шифрования генерируется случайный IV (16 байт), который добавляется в начало зашифрованного результата.
     *
     * @param plaintext Вектор исходных данных для шифрования.
     * @param key Бинарный ключ (32 байта).
     * @return std::vector<uint8_t> Вектор, содержащий IV (16 байт) + зашифрованные данные.
     * @throws std::runtime_error При неверном размере ключа или ошибках шифрования.
     */
    std::vector<uint8_t> encryptData(const std::vector<uint8_t>& plaintext, const std::vector<uint8_t>& key);
}
#endif // ENCRYPTION_H
