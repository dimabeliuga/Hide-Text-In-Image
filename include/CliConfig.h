#ifndef CLI_CONFIG_H
#define CLI_CONFIG_H

#include <string>
#include <iostream>

/**
 * @brief Singleton structure that stores command-line configuration parameters.
 * 
 * This structure holds user-provided arguments such as encryption mode,
 * decryption mode, input/output file paths, and passphrase.
 */
struct CliConfig {
    bool modeEncrypt = false;  ///< Encryption mode (extract hidden message).
    bool modeCrypt   = false;  ///< Embedding mode (hide message in the image).
    std::string textMessage;   ///< Message to be hidden in the image.
    std::string inFile;        ///< Input file path.
    std::string outFile;       ///< Output file path.
    std::string passphrase;    ///< Encryption passphrase.

    /**
     * @brief Returns a singleton instance of the CliConfig.
     * 
     * Ensures that only one instance of CliConfig is used throughout the program.
     * 
     * @return CliConfig& Reference to the singleton instance.
     */
    static CliConfig& getInstance() {
        static CliConfig config;
        return config;
    }

    // Disable copy constructor and assignment operator
    CliConfig(const CliConfig&) = delete;
    CliConfig& operator=(const CliConfig&) = delete;

private:
    /**
     * @brief Private constructor to enforce the singleton pattern.
     */
    CliConfig() {}
};

#endif // CLI_CONFIG_H
