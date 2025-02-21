#ifndef CLI_CONFIG_H
#define CLI_CONFIG_H

#include <string>
#include <iostream>

struct CliConfig{
    // Разбор аргументов
    bool modeEncrypt = false;  // режим извлечения
    bool modeCrypt   = false;  // режим встраивания
    std::string textMessage;
    std::string inFile;
    std::string outFile;
    std::string passphrase;

    static CliConfig& getInstance(){
        static CliConfig config;
        return config;
    }
    
    CliConfig(const CliConfig&) = delete;
    CliConfig& operator=(const CliConfig&) = delete;

private:
    CliConfig() {}
};

#endif