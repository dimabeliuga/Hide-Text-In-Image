#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>
#include <cstdint>
#include "external/logger.h"

namespace Utils {

    /**
     * @brief Вычисляет HMAC-SHA256 от данных с использованием заданного ключа.
     *
     * @param data Вектор данных для вычисления HMAC.
     * @param key Ключ для HMAC.
     * @return std::vector<uint8_t> Вычисленный HMAC (32 байта).
     * @throws std::runtime_error В случае ошибки вычисления HMAC.
     */
    std::vector<uint8_t> computeHMAC(const std::vector<uint8_t>& data, const std::vector<uint8_t>& key);

    /**
     * @brief Преобразует вектор байтов в строку в шестнадцатеричном представлении.
     *
     * @param data Вектор байтов.
     * @return std::string Строка в шестнадцатеричном виде.
     */
    std::string bytesToHex(const std::vector<uint8_t>& data);

    /**
     * @brief Преобразует строку в шестнадцатеричном представлении в вектор байтов.
     *
     * @param hex Строка с шестнадцатеричными данными.
     * @return std::vector<uint8_t> Вектор байтов.
     * @throws std::runtime_error Если строка имеет некорректный формат.
     */
    std::vector<uint8_t> hexToBytes(const std::string& hex);

    /**
     * @brief Генерирует криптографически стойкие случайные байты заданной длины.
     *
     * @param length Количество байтов для генерации.
     * @return std::vector<uint8_t> Сгенерированные байты.
     * @throws std::runtime_error Если генерация случайных байтов не удалась.
     */
    std::vector<uint8_t> getRandomBytes(size_t length);

} // namespace Utils

#endif // UTILS_H
