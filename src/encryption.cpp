#include "encryption.h"

#include <openssl/evp.h>
#include <openssl/rand.h>
#include <stdexcept>
#include <vector>

namespace Encryption {

std::vector<uint8_t> encryptData(const std::vector<uint8_t>& plaintext, const std::vector<uint8_t>& key) {
    // Проверка: ключ должен быть ровно 32 байта для AES-256
    if (key.size() != 32) {
        throw std::runtime_error("Key size must be 32 bytes for AES-256.");
    }

    // Создаём контекст для шифрования
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        throw std::runtime_error("Failed to create EVP_CIPHER_CTX.");
    }

    // Генерируем случайный IV длиной 16 байт
    unsigned char iv[16];
    if (RAND_bytes(iv, sizeof(iv)) != 1) {
        EVP_CIPHER_CTX_free(ctx);
        throw std::runtime_error("Failed to generate random IV.");
    }

    // Инициализируем контекст шифрования с алгоритмом AES-256-CBC
    if (EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, key.data(), iv) != 1) {
        EVP_CIPHER_CTX_free(ctx);
        throw std::runtime_error("EVP_EncryptInit_ex failed.");
    }

    // Выделяем буфер для зашифрованных данных (с запасом на возможный рост размера)
    int blockSize = EVP_CIPHER_block_size(EVP_aes_256_cbc());
    int maxCiphertextLen = plaintext.size() + blockSize;
    std::vector<uint8_t> ciphertext(maxCiphertextLen);

    int len = 0;
    // Шифруем данные
    if (EVP_EncryptUpdate(ctx, ciphertext.data(), &len, plaintext.data(), plaintext.size()) != 1) {
        EVP_CIPHER_CTX_free(ctx);
        throw std::runtime_error("EVP_EncryptUpdate failed.");
    }
    int ciphertextLen = len;

    // Завершаем шифрование (обработка последних блоков)
    if (EVP_EncryptFinal_ex(ctx, ciphertext.data() + len, &len) != 1) {
        EVP_CIPHER_CTX_free(ctx);
        throw std::runtime_error("EVP_EncryptFinal_ex failed.");
    }
    ciphertextLen += len;
    ciphertext.resize(ciphertextLen);

    // Освобождаем ресурсы
    EVP_CIPHER_CTX_free(ctx);

    // Формируем итоговый вектор: сначала IV, затем зашифрованные данные
    std::vector<uint8_t> output;
    output.insert(output.end(), iv, iv + sizeof(iv));
    output.insert(output.end(), ciphertext.begin(), ciphertext.end());

    return output;
}

std::vector<uint8_t> decryptData(const std::vector<uint8_t>& ciphertext, const std::vector<uint8_t>& key) {
    // Проверка: ключ должен быть ровно 32 байта для AES-256
    if (key.size() != 32) {
        throw std::runtime_error("Key size must be 32 bytes for AES-256.");
    }

    // Проверка: зашифрованные данные должны содержать как минимум IV (16 байт)
    if (ciphertext.size() < 16) {
        throw std::runtime_error("Ciphertext is too short, missing IV.");
    }

    // Извлекаем IV из первых 16 байт
    const unsigned char* iv = ciphertext.data();
    const unsigned char* encData = ciphertext.data() + 16;
    size_t encDataLen = ciphertext.size() - 16;

    // Создаём контекст для расшифрования
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        throw std::runtime_error("Failed to create EVP_CIPHER_CTX.");
    }

    // Инициализируем контекст для расшифрования с тем же алгоритмом AES-256-CBC
    if (EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, key.data(), iv) != 1) {
        EVP_CIPHER_CTX_free(ctx);
        throw std::runtime_error("EVP_DecryptInit_ex failed.");
    }

    // Выделяем буфер для расшифрованных данных
    std::vector<uint8_t> plaintext(encDataLen);
    int len = 0;
    // Расшифровываем данные
    if (EVP_DecryptUpdate(ctx, plaintext.data(), &len, encData, encDataLen) != 1) {
        EVP_CIPHER_CTX_free(ctx);
        throw std::runtime_error("EVP_DecryptUpdate failed.");
    }
    int plaintextLen = len;

    // Завершаем расшифрование
    if (EVP_DecryptFinal_ex(ctx, plaintext.data() + len, &len) != 1) {
        EVP_CIPHER_CTX_free(ctx);
        throw std::runtime_error("EVP_DecryptFinal_ex failed. Data may be corrupted or wrong key.");
    }
    plaintextLen += len;
    plaintext.resize(plaintextLen);

    // Освобождаем ресурсы
    EVP_CIPHER_CTX_free(ctx);

    return plaintext;
}

} // namespace Encryption
