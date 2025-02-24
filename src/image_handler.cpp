#include "image_handler.h"

#include <stdexcept>
#include <fstream>
#include <algorithm>

// Подключаем реализации stb_image и stb_image_write
#define STB_IMAGE_IMPLEMENTATION
#include "external/stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "external/stb_image_write.h"

namespace ImageHandler {

bool fileExists(const std::string& filename) {
    std::ifstream file(filename);
    return file.good();
}

bool isSupportedFormat(const std::string& filename) {
    std::string lowerFilename = filename;
    std::transform(lowerFilename.begin(), lowerFilename.end(), lowerFilename.begin(), ::tolower);
    // Проверяем последние 4 символа (например, ".png" или ".bmp")
    if (lowerFilename.size() < 4) return false;
    std::string ext = lowerFilename.substr(lowerFilename.size() - 4);
    return (ext == ".png" || ext == ".bmp");
}

Image loadImage(const std::string& filename) {
    if (!fileExists(filename)) {
        LOG_ERROR("The file: {} does not exist", filename);
        exit(EXIT_FAILURE);
    }
    if (!isSupportedFormat(filename)) {
        LOG_ERROR("Unsupported file format: {}", filename);
        exit(EXIT_FAILURE);
    }

    int width, height, channels;
    // Загружаем изображение с сохранением исходного количества каналов
    unsigned char* imgData = stbi_load(filename.c_str(), &width, &height, &channels, 0);
    if (!imgData) {
        LOG_ERROR("Failed to load the image: {}", filename);
        exit(EXIT_FAILURE);
    }

    size_t dataSize = static_cast<size_t>(width) * height * channels;
    std::vector<uint8_t> data(imgData, imgData + dataSize);

    // Освобождаем память, выделенную stb_image
    stbi_image_free(imgData);

    LOG_INFO("Image information was loaded from the image succesfully");
    return Image{ width, height, channels, data };
}

void saveImage(const std::string& filename, const Image& image) {
    if (!isSupportedFormat(filename)) {
        LOG_ERROR("Unsuported file format {}", filename);
        exit(EXIT_FAILURE);
    }

    std::string lowerFilename = filename;
    std::transform(lowerFilename.begin(), lowerFilename.end(), lowerFilename.begin(), ::tolower);
    bool success = false;

    if (lowerFilename.substr(lowerFilename.size() - 4) == ".png") {
        // Для PNG указываем ширину строки (stride)
        success = stbi_write_png(filename.c_str(), image.width, image.height, image.channels,
                                 image.data.data(), image.width * image.channels);
    } else if (lowerFilename.substr(lowerFilename.size() - 4) == ".bmp") {
        success = stbi_write_bmp(filename.c_str(), image.width, image.height, image.channels,
                                 image.data.data());
    }

    if (!success) {
        LOG_ERROR("Failed to save image");
        exit(EXIT_FAILURE);
    }
    LOG_INFO("The picture was saved in {}", filename);
}

} // namespace ImageHandler
