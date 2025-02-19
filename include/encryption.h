#ifndef ENCRYPTION_H
#define ENCRYPTION_H

#include <vector>
#include <cstdint>

// Все функции находятся в пространстве имён Encryption.
namespace Encryption {

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

} // namespace Encryption

#endif // ENCRYPTION_H
