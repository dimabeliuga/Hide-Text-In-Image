## -SteganoEncrypt ##
SteganoEncrypt is a C++ application designed to securely hide and extract encrypted text within images using advanced steganographic techniques. By combining robust AES-256-CBC encryption with LSB (Least Significant Bit) steganography, the tool ensures that sensitive messages remain confidential and are seamlessly integrated into BMP or PNG images.

## - Overview ##
SteganoEncrypt first encrypts the input text using AES-256-CBC, where the encryption key is derived from a user-provided passphrase using PBKDF2. The encrypted message is then embedded into an image by modifying the least significant bits of selected pixels. The embedding process uses a key-driven pseudo-random index generator to distribute message bits across the image, while non-essential pixels are randomly altered to obfuscate the hidden data further. The extraction process reverses these steps to retrieve and decrypt the hidden message.

## - Key Features ##
Robust Encryption:
Utilizes AES-256-CBC encryption to secure the input text. A strong binary key is generated from a passphrase using PBKDF2 with a random salt.

## - Steganographic Embedding: ##
Embeds encrypted message bits into images by modifying the LSB of randomly selected pixels. This random distribution increases the security and obscurity of the hidden message.

## - Supported Image Formats: ##
Works with BMP and PNG images to ensure that compression artifacts do not corrupt the hidden data.

## - Efficient Image Processing: ##
Leverages libraries such as stb_image and stb_image_write for fast and reliable image loading and saving.

## - Integrated Logging: ##
Uses spdlog for structured and configurable logging, which aids in debugging and monitoring application behavior.

## - Build and Usage ##
SteganoEncrypt is built using CMake and requires OpenSSL for encryption as well as spdlog for logging. Detailed instructions for building and running the application can be found in the project documentation.

## - Future Enhancements ##
1. ** Performance Optimization: **
Explore techniques to further accelerate the program, such as advanced parallelization of steganographic operations and optimizing random index selection to reduce overhead.

2. Cross-Platform Portability:
Improve portability by refining platform-specific code and adopting more standardized libraries and build systems to ensure seamless operation on Windows, Linux, and macOS.

3. Increased Robustness:
Enhance the application's resistance to attacks and operational errors by integrating stronger error handling, additional data integrity checks, and improved encryption parameter management.

SteganoEncrypt combines modern cryptography with innovative steganography, making it a practical and secure solution for covert communication and data protection. Contributions, suggestions, and feedback are welcome!

Happy Hiding!
