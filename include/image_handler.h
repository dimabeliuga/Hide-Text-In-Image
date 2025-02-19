#ifndef IMAGE_HANDLER_H
#define IMAGE_HANDLER_H

#include <string>
#include <vector>
#include <cstdint>

namespace ImageHandler {

    // Структура для хранения данных изображения
    struct Image {
        int width;                // Ширина изображения
        int height;               // Высота изображения
        int channels;             // Количество каналов (например, 3 для RGB, 4 для RGBA)
        std::vector<uint8_t> data; // Сырые пиксельные данные
    };

    /**
     * @brief Проверяет, существует ли файл.
     *
     * @param filename Путь к файлу.
     * @return true, если файл существует, иначе false.
     */
    bool fileExists(const std::string& filename);

    /**
     * @brief Проверяет, поддерживается ли формат файла (только bmp и png).
     *
     * @param filename Путь к файлу.
     * @return true, если формат поддерживается, иначе false.
     */
    bool isSupportedFormat(const std::string& filename);

    /**
     * @brief Загружает изображение из файла.
     *
     * Функция проверяет существование файла и его формат, затем загружает изображение и возвращает его в виде структуры Image.
     *
     * @param filename Путь к файлу изображения.
     * @return Image Загруженное изображение.
     * @throws std::runtime_error Если файл не существует, формат неподдерживаемый или произошла ошибка при загрузке.
     */
    Image loadImage(const std::string& filename);

    /**
     * @brief Сохраняет изображение в файл.
     *
     * Выбор функции сохранения (PNG или BMP) определяется по расширению файла.
     *
     * @param filename Путь к файлу для сохранения изображения.
     * @param image Структура Image с данными изображения.
     * @throws std::runtime_error Если формат файла неподдерживаемый или произошла ошибка при сохранении.
     */
    void saveImage(const std::string& filename, const Image& image);

} // namespace ImageHandler

#endif // IMAGE_HANDLER_H
