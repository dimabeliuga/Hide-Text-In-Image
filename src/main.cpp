#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <sstream>
#include <cstdint>
#include <cstring>

#define SPDLOG_LEVEL_TRACE

#include "encryption/encryption.h"
#include "encryption/decrytpion.h"
#include "external/logger.h"
#include "external/stb_image_write.h"
#include "stegano.h"
#include "CliParser.h"

// 1) финальная идея это сделать программу которая будет работать даже с пользовательскими алгоритмами шифрования
// 2) сделать консольный парсе +
// 3) добавить свой логер +
// 4) добавить параметры (1) если выходной файл не указан, то предложить куда сохранять + 2) провекрка на существование такого же выходного файла
// 5) можно добавить паралельность для записи байтов в файл +
// 6) добавить чтобы больше параметров выбирал пользователь 
// 7) написать еще несколько библиотек с разными алгоритмами шифрования
// 8) сделать сервер для обмена этими фотографиями
// 

int main(int argc, char* argv[]) {
    setup_logger();

    auto& config = CliParser::parse(argc, argv);
    
    // Конвертируем passphrase в вектор байтов для стеганографии (используем ASCII представление)
    std::vector<uint8_t> steganoKey = DataConversion::stringToBytes(config.passphrase);
    
    if (config.modeCrypt) {
        LOG_INFO("--------------Crypt mode start---------------");
        // Загрузка исходного изображения
        ImageHandler::Image image = ImageHandler::loadImage(config.inFile);

        auto embededText = Encryption::getReadyToEmbedText(config);

        // Встраиваем данные в изображение
        Stegano::embedData(image, embededText, steganoKey);

        // Сохраняем изменённое изображение
        ImageHandler::saveImage(config.outFile, image);
        LOG_INFO("-----------crypto mode end ----------");
    } 
    else if (config.modeEncrypt) {
        LOG_INFO("-----------encrypto mode start-------");
        // Режим извлечения
        ImageHandler::Image image = ImageHandler::loadImage(config.inFile);
        
        std::string decryptedMessage = Decryption::getDecryptedMessage(config, image, steganoKey);
        std::cout << decryptedMessage << std::endl;
        LOG_INFO("----------encrypto mode finish--------");
    }

    return 0;
}
