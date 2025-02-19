#ifndef KEY_DERIVATION_H
#define KEY_DERIVATION_H

#include <vector>
#include <cstdint>
#include <string>

namespace KeyDerivation {

    /**
     * @brief Выводит бинарный ключ из строкового пароля с использованием PBKDF2 и HMAC-SHA256.
     *
     * @param passphrase Строковый ключ, введённый пользователем.
     * @param salt Соль для повышения стойкости вывода ключа.
     * @param iterations Количество итераций (рекомендуется не менее 10000).
     * @param keyLength Желаемая длина бинарного ключа в байтах.
     * @return std::vector<uint8_t> Выведенный бинарный ключ.
     * @throws std::runtime_error Если вывод ключа не удался.
     */
    std::vector<uint8_t> deriveKey(const std::string& passphrase, 
                                   const std::vector<uint8_t>& salt, 
                                   int iterations, 
                                   size_t keyLength);

    /**
     * @brief Генерирует криптографически стойкую случайную соль заданной длины.
     *
     * @param saltLength Длина соли в байтах.
     * @return std::vector<uint8_t> Сгенерированная соль.
     * @throws std::runtime_error Если не удалось сгенерировать соль.
     */
    std::vector<uint8_t> generateSalt(size_t saltLength);

} // namespace KeyDerivation

#endif // KEY_DERIVATION_H
