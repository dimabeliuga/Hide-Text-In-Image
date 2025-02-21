#include "encryption/decrytpion.h"

#include <openssl/evp.h>
#include <openssl/rand.h>
#include <cstdint>
#include <vector>

namespace Decryption{
    
    std::string getDecryptedMessage(const CliConfig& config, ImageHandler::Image& image, std::vector<uint8_t>& steganoKey){
    // Сначала извлекаем заголовок (4 байта) из изображения
    std::vector<uint8_t> header = Stegano::extractData(image, DataConversion::HEADER_SIZE, steganoKey);
    uint32_t containerLength = DataConversion::bytesToUint32(header);

    // Теперь извлекаем полный контейнер (заголовок уже извлечён, поэтому общее количество байтов: HEADER_SIZE + containerLength)
    size_t totalMessageLength = DataConversion::HEADER_SIZE + containerLength;
    std::vector<uint8_t> fullMessage = Stegano::extractData(image, totalMessageLength, steganoKey);

    // Проверяем, что извлечённый заголовок совпадает с тем, что мы уже прочитали
    std::vector<uint8_t> extractedHeader(fullMessage.begin(), fullMessage.begin() + DataConversion::HEADER_SIZE);
    if (extractedHeader != header) {
        LOG_ERROR("Extracting error: the title does not match");
        exit(EXIT_FAILURE);
    }

    // Извлекаем контейнер (оставшиеся байты)
    std::vector<uint8_t> container(fullMessage.begin() + DataConversion::HEADER_SIZE, fullMessage.end());
    if (container.size() < DataConversion::SALT_SIZE) {
        LOG_ERROR("Extacted container is too small");
        exit(EXIT_FAILURE);
    }

    // Первая SALT_SIZE байт – это соль, остальное – зашифрованные данные
    std::vector<uint8_t> salt(container.begin(), container.begin() + DataConversion::SALT_SIZE);
    std::vector<uint8_t> encryptedData(container.begin() + DataConversion::SALT_SIZE, container.end());


    // Вычисляем бинарный ключ для шифрования с использованием извлечённой соли
    std::vector<uint8_t> derivedKey = KeyDerivation::deriveKey(config.passphrase, salt, DataConversion::KDF_ITERATIONS, 32);

    // Дешифруем сообщение
    std::vector<uint8_t> decryptedData = decryptData(encryptedData, derivedKey);
    std::string decryptedMessage(decryptedData.begin(), decryptedData.end());
    
    LOG_INFO("Message was successfuly extracted and decrypted from the image");
    return decryptedMessage;
    }
}

namespace {
    std::vector<uint8_t> decryptData(const std::vector<uint8_t>& ciphertext, const std::vector<uint8_t>& key) {
        // Проверка: ключ должен быть ровно 32 байта для AES-256
        if (key.size() != 32) {
            LOG_ERROR("Key size must be 32 bytes for AES-256");
            exit(EXIT_FAILURE);
        }

        // Проверка: зашифрованные данные должны содержать как минимум IV (16 байт)
        if (ciphertext.size() < 16) {
            LOG_ERROR("Ciphertext is too short, missing IV");
            exit(EXIT_FAILURE);
        }

        // Извлекаем IV из первых 16 байт
        const unsigned char* iv = ciphertext.data();
        const unsigned char* encData = ciphertext.data() + 16;
        size_t encDataLen = ciphertext.size() - 16;

        // Создаём контекст для расшифрования
        EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
        if (!ctx) {
            LOG_ERROR("Failed to create EVP_CIPHER_CTX");
            exit(EXIT_FAILURE);
        }

        // Инициализируем контекст для расшифрования с тем же алгоритмом AES-256-CBC
        if (EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, key.data(), iv) != 1) {
            EVP_CIPHER_CTX_free(ctx);
            LOG_ERROR("EVP_DecryptInit_ex failed");
            exit(EXIT_FAILURE);
        }

        // Выделяем буфер для расшифрованных данных
        std::vector<uint8_t> plaintext(encDataLen);
        int len = 0;
        // Расшифровываем данные
        if (EVP_DecryptUpdate(ctx, plaintext.data(), &len, encData, encDataLen) != 1) {
            EVP_CIPHER_CTX_free(ctx);
            LOG_ERROR("EVP_DecryptUpdate failed");
            exit(EXIT_FAILURE);
        }
        int plaintextLen = len;

        // Завершаем расшифрование
        if (EVP_DecryptFinal_ex(ctx, plaintext.data() + len, &len) != 1) {
            EVP_CIPHER_CTX_free(ctx);
            LOG_ERROR("EVP_DecryptFinal_ex failed. Data may be corrupted or wrong key");
            exit(EXIT_FAILURE);
        }
        plaintextLen += len;
        plaintext.resize(plaintextLen);

        // Освобождаем ресурсы
        EVP_CIPHER_CTX_free(ctx);

        LOG_INFO("The data was decrypted successfuly");
        return plaintext;
    }
}
