#include "encryption/encryption.h"
#include "external/logger.h"
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <stdexcept>
#include <vector>

namespace Encryption {
    std::vector<uint8_t> getReadyToEmbedText(CliConfig& config){    
        // Генерируем соль и выводим её (соль не скрывается для расшифровки, она будет включена в контейнер)
        std::vector<uint8_t> salt = KeyDerivation::generateSalt(DataConversion::SALT_SIZE);

        // Производим вывод бинарного ключа для шифрования через KDF (выход 32 байта)
        std::vector<uint8_t> derivedKey = KeyDerivation::deriveKey(config.passphrase, salt, DataConversion::KDF_ITERATIONS, 32);

        // Шифруем сообщение (преобразуем текст в вектор байтов)
        std::vector<uint8_t> plainText(config.textMessage.begin(), config.textMessage.end());
        std::vector<uint8_t> encryptedData = encryptData(plainText, derivedKey);

        // Формируем контейнер: сначала соль, затем зашифрованные данные
        std::vector<uint8_t> container;
        container.insert(container.end(), salt.begin(), salt.end());
        container.insert(container.end(), encryptedData.begin(), encryptedData.end());

        // Формируем заголовок: 4 байта, содержащие длину контейнера
        uint32_t containerLength = static_cast<uint32_t>(container.size());
        std::vector<uint8_t> header = DataConversion::uint32ToBytes(containerLength);

        // Итоговое сообщение для внедрения: заголовок + контейнер
        std::vector<uint8_t> finalMessage;
        finalMessage.insert(finalMessage.end(), header.begin(), header.end());
        finalMessage.insert(finalMessage.end(), container.begin(), container.end());
        
        LOG_INFO("String to embed was comiled successfuly");
        return finalMessage;
    }    
}

namespace {
    std::vector<uint8_t> encryptData(const std::vector<uint8_t>& plaintext, const std::vector<uint8_t>& key) {
        // Проверка: ключ должен быть ровно 32 байта для AES-256
        if (key.size() != 32) {
            LOG_ERROR("Key size must be 32 bytes for AES-256");
            exit(EXIT_FAILURE);
        }

        // Создаём контекст для шифрования
        EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
        if (!ctx) {
            LOG_ERROR("Failed to create EVP_CIPHER_CTX");
            exit(EXIT_FAILURE);
        }

        // Генерируем случайный IV длиной 16 байт
        unsigned char iv[16];
        if (RAND_bytes(iv, sizeof(iv)) != 1) {
            EVP_CIPHER_CTX_free(ctx);
            LOG_ERROR("Failed to generate random IV");
            exit(EXIT_FAILURE);
        }

        // Инициализируем контекст шифрования с алгоритмом AES-256-CBC
        if (EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, key.data(), iv) != 1) {
            EVP_CIPHER_CTX_free(ctx);
            LOG_ERROR("EVP_EncryptInit_ex failed");
            exit(EXIT_FAILURE);
        }

        // Выделяем буфер для зашифрованных данных (с запасом на возможный рост размера)
        int blockSize = EVP_CIPHER_block_size(EVP_aes_256_cbc());
        size_t maxCiphertextLen = plaintext.size() + blockSize;
        std::vector<uint8_t> ciphertext(maxCiphertextLen);

        int len = 0;
        // Шифруем данные
        if (EVP_EncryptUpdate(ctx, ciphertext.data(), &len, plaintext.data(), plaintext.size()) != 1) {
            EVP_CIPHER_CTX_free(ctx);
            LOG_ERROR("EVP_EncryptUpdate failed");
            exit(EXIT_FAILURE);
        }
        int ciphertextLen = len;

        // Завершаем шифрование (обработка последних блоков)
        if (EVP_EncryptFinal_ex(ctx, ciphertext.data() + len, &len) != 1) {
            EVP_CIPHER_CTX_free(ctx);
            LOG_ERROR("EVP_EncryptFinal_ex failed");
            exit(EXIT_FAILURE);
        }
        ciphertextLen += len;
        ciphertext.resize(ciphertextLen);

        // Освобождаем ресурсы
        EVP_CIPHER_CTX_free(ctx);

        // Формируем итоговый вектор: сначала IV, затем зашифрованные данные
        std::vector<uint8_t> output;
        output.insert(output.end(), iv, iv + sizeof(iv));
        output.insert(output.end(), ciphertext.begin(), ciphertext.end());

        LOG_INFO("Cryption went successful");
        return output;
    }
} // namespace Encryption
