#include "stegano.h"

#include <random>
#include <algorithm>
#include <stdexcept>
#include <cstdint>
#include <vector>
#include <thread>

namespace Stegano {

// Вспомогательная функция для генерации перестановки индексов [0, n-1] на основе ключа.
static std::vector<size_t> generateShuffledIndices(size_t n, const std::vector<uint8_t>& key) {
    std::vector<size_t> indices(n);
    for (size_t i = 0; i < n; i++) {
        indices[i] = i;
    }
    // Преобразуем ключ в вектор чисел для инициализации seed_seq
    std::vector<unsigned int> seedData(key.begin(), key.end());
    std::seed_seq seedSeq(seedData.begin(), seedData.end());
    std::mt19937 rng(seedSeq);
    std::shuffle(indices.begin(), indices.end(), rng);
    
    LOG_INFO("Shuffled Indices were compiled successfuly");
    return indices;
}

void embedData(ImageHandler::Image& image, const std::vector<uint8_t>& message, const std::vector<uint8_t>& key) {
    // Количество доступных байтов (каждый канал - 1 байт)
    size_t totalChannels = image.data.size();
    size_t totalBits = totalChannels; // 1 бит на канал
    size_t messageBits = message.size() * 8;

    if (messageBits > totalBits) {
        LOG_ERROR("The message is too big. It is impossible to place the all text into the picture");
        exit(EXIT_FAILURE);
    }

    // Генерируем псевдослучайную перестановку индексов на основе ключа.
    std::vector<size_t> shuffledIndices = generateShuffledIndices(totalBits, key);

    std::thread fillUnecessaryBits([&](){
        // Для оставшихся позиций производим случайное изменение LSB для маскировки.
        // Используем тот же ключ для инициализации генератора.
        std::vector<unsigned int> seedData(key.begin(), key.end());
        std::seed_seq seedSeq(seedData.begin(), seedData.end());
        std::mt19937 rng(seedSeq);
        std::uniform_int_distribution<int> coinFlip(0, 1);

        for (size_t i = messageBits; i < totalBits; i++) {
            size_t dataIndex = shuffledIndices[i];
            uint8_t currentValue = image.data[dataIndex];
            int direction = (coinFlip(rng) == 0) ? -1 : 1;

            // Обеспечиваем, чтобы значение не вышло за пределы [0, 255]
            if (currentValue == 0) {
                direction = 1;
            } else if (currentValue == 255) {
                direction = -1;
            }
            int newValue = static_cast<int>(currentValue) + direction;
            image.data[dataIndex] = static_cast<uint8_t>(newValue);
        }
    });

    // Встраиваем биты сообщения в выбранные позиции.
    for (size_t bitIndex = 0; bitIndex < messageBits; bitIndex++) {
        size_t dataIndex = shuffledIndices[bitIndex];
        size_t byteIndex = bitIndex / 8;
        size_t bitInByte = 7 - (bitIndex % 8); // Берём бит с старшего разряда

        uint8_t bitValue = (message[byteIndex] >> bitInByte) & 0x01;
        // Устанавливаем LSB выбранного байта в значение bitValue.
        image.data[dataIndex] = (image.data[dataIndex] & 0xFE) | bitValue;
    }

    if(fillUnecessaryBits.joinable()){
        fillUnecessaryBits.join();
        LOG_INFO("fillUnecessaryBits thread was joined");
    }
    LOG_INFO("The data was embeded in the picture");
}

std::vector<uint8_t> extractData(const ImageHandler::Image& image, size_t messageLength, const std::vector<uint8_t>& key) {
    size_t totalChannels = image.data.size();
    size_t totalBits = totalChannels;
    size_t messageBits = messageLength * 8;

    if (messageBits > totalBits) {
        LOG_ERROR("The specified message length exceeds the image capacity");
        exit(EXIT_FAILURE);
    }

    // Генерируем ту же последовательность перестановки.
    std::vector<size_t> shuffledIndices = generateShuffledIndices(totalBits, key);

    std::vector<uint8_t> message(messageLength, 0);
    for (size_t bitIndex = 0; bitIndex < messageBits; bitIndex++) {
        size_t dataIndex = shuffledIndices[bitIndex];
        uint8_t bitValue = image.data[dataIndex] & 0x01;

        size_t byteIndex = bitIndex / 8;
        size_t bitInByte = 7 - (bitIndex % 8);
        message[byteIndex] |= (bitValue << bitInByte);
    }

    LOG_INFO("The data was extracted from the file");
    return message;
}

} // namespace Stegano
