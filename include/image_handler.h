#ifndef IMAGE_HANDLER_H
#define IMAGE_HANDLER_H

#include <string>
#include <vector>
#include <cstdint>
#include "external/logger.h"

namespace ImageHandler {

    /**
     * @brief Structure for storing image data.
     * 
     * Contains essential metadata about the image, such as width, height, number of color channels,
     * and raw pixel data.
     */
    struct Image {
        int width;                ///< Image width.
        int height;               ///< Image height.
        int channels;             ///< Number of color channels (e.g., 3 for RGB, 4 for RGBA).
        std::vector<uint8_t> data; ///< Raw pixel data.
    };

    /**
     * @brief Checks whether a file exists.
     * 
     * @param filename Path to the file.
     * @return true if the file exists, false otherwise.
     */
    bool fileExists(const std::string& filename);

    /**
     * @brief Checks if the file format is supported (only BMP and PNG).
     * 
     * @param filename Path to the file.
     * @return true if the format is supported, false otherwise.
     */
    bool isSupportedFormat(const std::string& filename);

    /**
     * @brief Loads an image from a file.
     * 
     * This function verifies the file existence and format before loading the image
     * and returning it as an `Image` structure.
     * 
     * @param filename Path to the image file.
     * @return Image Loaded image.
     * @throws std::runtime_error If the file does not exist, the format is unsupported, or an error occurs while loading.
     */
    Image loadImage(const std::string& filename);

    /**
     * @brief Saves an image to a file.
     * 
     * The function determines the save format (PNG or BMP) based on the file extension.
     * 
     * @param filename Path to the output file.
     * @param image The `Image` structure containing image data.
     * @throws std::runtime_error If the file format is unsupported or an error occurs while saving.
     */
    void saveImage(const std::string& filename, const Image& image);

} // namespace ImageHandler

#endif // IMAGE_HANDLER_H
