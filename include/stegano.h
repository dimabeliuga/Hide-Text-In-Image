#ifndef STEGANO_H
#define STEGANO_H

#include <vector>
#include <cstdint>
#include "image_handler.h"
#include "external/logger.h"

namespace Stegano {

    /**
     * @brief Embeds data into an image using a key to generate random positions.
     * 
     * The function modifies the image in place by setting the least significant bit (LSB)
     * of selected pixels according to the message bits. For additional obfuscation,
     * non-essential pixels undergo random LSB modification (±1).
     * 
     * @param image The image where the data will be embedded.
     * @param message A byte array representing the data to be embedded.
     * @param key A binary key used to initialize the random number generator.
     * @throws std::runtime_error If the message is too large for the given image.
     */
    void embedData(ImageHandler::Image& image, const std::vector<uint8_t>& message, const std::vector<uint8_t>& key);

    /**
     * @brief Extracts data from an image using a key to generate the sequence of positions.
     * 
     * This function retrieves the hidden message by extracting bits from specific pixel positions,
     * which were previously determined using the given key.
     * 
     * @param image The image from which the message will be extracted.
     * @param messageLength The length of the extracted message in bytes.
     * @param key A binary key used to initialize the random number generator.
     * @return std::vector<uint8_t> The extracted message.
     * @throws std::runtime_error If the specified message length exceeds the image's capacity.
     */
    std::vector<uint8_t> extractData(const ImageHandler::Image& image, size_t messageLength, const std::vector<uint8_t>& key);

} // namespace Stegano

#endif // STEGANO_H
