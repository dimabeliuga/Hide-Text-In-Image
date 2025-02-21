#ifndef STEGANO_H
#define STEGANO_H

#include <vector>
#include <cstdint>
#include "image_handler.h"
#include "external/logger.h"


namespace Stegano {

    /**
     * @brief Встраивает данные в изображение, используя ключ для генерации случайных позиций.
     *
     * Функция изменяет изображение на месте, устанавливая LSB пикселей в соответствии с битами сообщения,
     * а для остальных пикселей производит случайное изменение LSB (±1) для маскировки.
     *
     * @param image Изображение, в которое будут встроены данные.
     * @param message Данные (массив байтов), которые необходимо встроить.
     * @param key Бинарный ключ для инициализации генератора случайных чисел.
     * @throws std::runtime_error Если сообщение слишком велико для данного изображения.
     */
    void embedData(ImageHandler::Image& image, const std::vector<uint8_t>& message, const std::vector<uint8_t>& key);

    /**
     * @brief Извлекает данные из изображения, используя ключ для генерации последовательности позиций.
     *
     * @param image Изображение, из которого будет извлечено сообщение.
     * @param messageLength Длина извлекаемого сообщения в байтах.
     * @param key Бинарный ключ для инициализации генератора случайных чисел.
     * @return std::vector<uint8_t> Извлечённое сообщение.
     * @throws std::runtime_error Если указанная длина сообщения превышает вместимость изображения.
     */
    std::vector<uint8_t> extractData(const ImageHandler::Image& image, size_t messageLength, const std::vector<uint8_t>& key);

} // namespace Stegano

#endif // STEGANO_H
