#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <sstream>
#include <cstdint>
#include <cstring>

#include "encryption.h"
#include "key_derivation.h"
#include "image_handler.h"
#include "stegano.h"
#include "utils.h"

// Размеры для контейнера
constexpr size_t HEADER_SIZE = 4;    // 4 байта для хранения длины контейнера
constexpr size_t SALT_SIZE   = 16;   // 16 байт соли для KDF
constexpr int KDF_ITERATIONS = 10000;  // Количество итераций для PBKDF2

// Преобразование строки в вектор байтов (ASCII)
std::vector<uint8_t> stringToBytes(const std::string& str) {
    return std::vector<uint8_t>(str.begin(), str.end());
}

// Преобразование uint32_t в 4-байтный big-endian массив
std::vector<uint8_t> uint32ToBytes(uint32_t value) {
    std::vector<uint8_t> bytes(4);
    bytes[0] = (value >> 24) & 0xFF;
    bytes[1] = (value >> 16) & 0xFF;
    bytes[2] = (value >> 8)  & 0xFF;
    bytes[3] = value & 0xFF;
    return bytes;
}

// Преобразование 4-байтового big-endian массива в uint32_t
uint32_t bytesToUint32(const std::vector<uint8_t>& bytes) {
    if (bytes.size() != 4) {
        throw std::runtime_error("Неверный размер заголовка для uint32_t.");
    }
    uint32_t value = 0;
    value |= (static_cast<uint32_t>(bytes[0]) << 24);
    value |= (static_cast<uint32_t>(bytes[1]) << 16);
    value |= (static_cast<uint32_t>(bytes[2]) << 8);
    value |= (static_cast<uint32_t>(bytes[3]));
    return value;
}

void printUsage(const std::string& progName) {
    std::cout << "Использование:\n"
              << "  " << progName << " --crypt --text \"сообщение\" --in входное_изображение --out выходное_изображение [--key \"пароль\"]\n"
              << "  " << progName << " --encrypt --in входное_изображение [--key \"пароль\"]\n";
}

int main(int argc, char* argv[]) {
    try {
        if (argc < 3) {
            printUsage(argv[0]);
            return EXIT_FAILURE;
        }

        // Разбор аргументов
        bool modeEncrypt = false;  // режим извлечения
        bool modeCrypt   = false;  // режим встраивания
        std::string textMessage;
        std::string inFile;
        std::string outFile;
        std::string passphrase;

        for (int i = 1; i < argc; ++i) {
            std::string arg = argv[i];
            if (arg == "--crypt") {
                modeCrypt = true;
            } else if (arg == "--encrypt") {
                modeEncrypt = true;
            } else if (arg == "--text") {
                if (i + 1 < argc) {
                    textMessage = argv[++i];
                } else {
                    throw std::runtime_error("Отсутствует значение для параметра --text.");
                }
            } else if (arg == "--in") {
                if (i + 1 < argc) {
                    inFile = argv[++i];
                } else {
                    throw std::runtime_error("Отсутствует значение для параметра --in.");
                }
            } else if (arg == "--out") {
                if (i + 1 < argc) {
                    outFile = argv[++i];
                } else {
                    throw std::runtime_error("Отсутствует значение для параметра --out.");
                }
            } else if (arg == "--key") {
                if (i + 1 < argc) {
                    passphrase = argv[++i];
                } else {
                    throw std::runtime_error("Отсутствует значение для параметра --key.");
                }
            } else {
                // Неизвестный параметр
                throw std::runtime_error("Неизвестный параметр: " + arg);
            }
        }

        if (modeCrypt == modeEncrypt) {
            // Должен быть выбран ровно один режим
            throw std::runtime_error("Выберите ровно один режим: --crypt или --encrypt.");
        }

        // Проверка обязательных параметров
        if (inFile.empty()) {
            throw std::runtime_error("Параметр --in обязателен.");
        }
        if (modeCrypt) {
            if (textMessage.empty()) {
                throw std::runtime_error("В режиме --crypt параметр --text обязателен.");
            }
            if (outFile.empty()) {
                throw std::runtime_error("В режиме --crypt параметр --out обязателен.");
            }
        }
        if (modeEncrypt && passphrase.empty()) {
            throw std::runtime_error("В режиме --encrypt параметр --key обязателен.");
        }

        // Для генерации перестановки в steganography используем ключ, полученный путём преобразования passphrase в байты.
        // Если пароль не задан в режиме шифрования, предложим сгенерировать надёжный.
        if (passphrase.empty() && modeCrypt) {
            // Предлагаем сгенерировать случайный пароль
            std::vector<uint8_t> randomKey = Utils::getRandomBytes(16); // например, 16 байт
            passphrase = Utils::bytesToHex(randomKey);
            std::cout << "Ключ не был указан. Сгенерирован надёжный ключ: " << passphrase << "\n"
                      << "Сохраните его для последующей расшифровки. Если не согласны, завершите выполнение программы.\n";
            std::cout << "Нажмите Enter для продолжения...";
            std::cin.ignore();
        }

        // Конвертируем passphrase в вектор байтов для стеганографии (используем ASCII представление)
        std::vector<uint8_t> steganoKey = stringToBytes(passphrase);

        if (modeCrypt) {
            // Загрузка исходного изображения
            ImageHandler::Image image = ImageHandler::loadImage(inFile);
            Utils::logInfo("Изображение успешно загружено.");

            // Генерируем соль и выводим её (соль не скрывается для расшифровки, она будет включена в контейнер)
            std::vector<uint8_t> salt = KeyDerivation::generateSalt(SALT_SIZE);

            // Выводим информацию о соли (в hex) для отладки, если нужно:
            Utils::logInfo("Соль сгенерирована.");

            // Производим вывод бинарного ключа для шифрования через KDF (выход 32 байта)
            std::vector<uint8_t> derivedKey = KeyDerivation::deriveKey(passphrase, salt, KDF_ITERATIONS, 32);

            // Шифруем сообщение (преобразуем текст в вектор байтов)
            std::vector<uint8_t> plainText(textMessage.begin(), textMessage.end());
            std::vector<uint8_t> encryptedData = Encryption::encryptData(plainText, derivedKey);
            Utils::logInfo("Сообщение успешно зашифровано.");

            // Формируем контейнер: сначала соль, затем зашифрованные данные
            std::vector<uint8_t> container;
            container.insert(container.end(), salt.begin(), salt.end());
            container.insert(container.end(), encryptedData.begin(), encryptedData.end());

            // Формируем заголовок: 4 байта, содержащие длину контейнера
            uint32_t containerLength = static_cast<uint32_t>(container.size());
            std::vector<uint8_t> header = uint32ToBytes(containerLength);

            // Итоговое сообщение для внедрения: заголовок + контейнер
            std::vector<uint8_t> finalMessage;
            finalMessage.insert(finalMessage.end(), header.begin(), header.end());
            finalMessage.insert(finalMessage.end(), container.begin(), container.end());

            // Проверяем вместимость изображения (количество доступных бит = количество байтов в image.data)
            size_t availableBits = image.data.size(); // 1 бит на канал
            size_t requiredBits = finalMessage.size() * 8;
            if (requiredBits > availableBits) {
                throw std::runtime_error("Изображение недостаточно велико для внедрения сообщения.");
            }

            // Встраиваем данные в изображение
            Stegano::embedData(image, finalMessage, steganoKey);
            Utils::logInfo("Данные успешно внедрены в изображение.");

            // Сохраняем изменённое изображение
            ImageHandler::saveImage(outFile, image);
            Utils::logInfo("Изображение сохранено: " + outFile);
        } 
        else if (modeEncrypt) {
            // Режим извлечения
            ImageHandler::Image image = ImageHandler::loadImage(inFile);
            Utils::logInfo("Изображение успешно загружено.");

            // Сначала извлекаем заголовок (4 байта) из изображения
            std::vector<uint8_t> header = Stegano::extractData(image, HEADER_SIZE, steganoKey);
            uint32_t containerLength = bytesToUint32(header);
            Utils::logInfo("Длина контейнера: " + std::to_string(containerLength) + " байт.");

            // Теперь извлекаем полный контейнер (заголовок уже извлечён, поэтому общее количество байтов: HEADER_SIZE + containerLength)
            size_t totalMessageLength = HEADER_SIZE + containerLength;
            std::vector<uint8_t> fullMessage = Stegano::extractData(image, totalMessageLength, steganoKey);

            // Проверяем, что извлечённый заголовок совпадает с тем, что мы уже прочитали
            std::vector<uint8_t> extractedHeader(fullMessage.begin(), fullMessage.begin() + HEADER_SIZE);
            if (extractedHeader != header) {
                throw std::runtime_error("Ошибка извлечения: заголовок не совпадает.");
            }

            // Извлекаем контейнер (оставшиеся байты)
            std::vector<uint8_t> container(fullMessage.begin() + HEADER_SIZE, fullMessage.end());
            if (container.size() < SALT_SIZE) {
                throw std::runtime_error("Извлечённый контейнер слишком мал.");
            }

            // Первая SALT_SIZE байт – это соль, остальное – зашифрованные данные
            std::vector<uint8_t> salt(container.begin(), container.begin() + SALT_SIZE);
            std::vector<uint8_t> encryptedData(container.begin() + SALT_SIZE, container.end());

            // Выводим информацию о соли (можно для отладки)
            Utils::logInfo("Соль извлечена из контейнера.");

            // Вычисляем бинарный ключ для шифрования с использованием извлечённой соли
            std::vector<uint8_t> derivedKey = KeyDerivation::deriveKey(passphrase, salt, KDF_ITERATIONS, 32);

            // Дешифруем сообщение
            std::vector<uint8_t> decryptedData = Encryption::decryptData(encryptedData, derivedKey);
            std::string decryptedMessage(decryptedData.begin(), decryptedData.end());
            Utils::logInfo("Сообщение успешно расшифровано:");
            std::cout << decryptedMessage << std::endl;
        }
    }
    catch (const std::exception& ex) {
        Utils::logError(ex.what());
        return 1;
    }

    return 0;
}
