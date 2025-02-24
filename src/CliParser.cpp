#include "CliParser.h"
#include "external/logger.h"

#include "encryption/utils.h"
#include <iostream>
#include <filesystem>

std::string CliParser::errorMessage;

void CliParser::getErrorMessage() {
    std::cerr << errorMessage << "\n";
}

void CliParser::printUsage() {
    std::cout << "Using:\n"
              << " --crypt --text \"message\" --in input_image_path --out output_image_path [--key \"password\"]\n"
              << " --encrypt --in input_image_path --key \"password\"\n";
}

CliConfig& CliParser::parse(int argc, char** argv){
    CliConfig& config = CliConfig::getInstance();
    if(!extractCommandLineArguments(argc, argv, config)){
        getErrorMessage();
        printUsage();
        exit(EXIT_FAILURE);
    }

    if(config.passphrase.empty() && config.modeCrypt){
        // Для генерации перестановки в steganography используем ключ, полученный путём преобразования passphrase в байты.
        // Если пароль не задан в режиме шифрования, предложим сгенерировать надёжный.
        generatePassphrase(config.passphrase);
    }
    
    if(config.modeCrypt){
        validateOutputPath(config.outFile);
    }

    return config;
}

bool CliParser::extractCommandLineArguments(int argc, char** argv, CliConfig& config){
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--crypt") {
            config.modeCrypt = true;
        } else if (arg == "--encrypt") {
            config.modeEncrypt = true;
        } else if (arg == "--text") {
            if (i + 1 < argc) {
                config.textMessage = argv[++i];
            } else {
                errorMessage = "Error: after the flag --text, there should be text that will be hidden in the picture";
                return false;
            }
        } else if (arg == "--in") {
            if (i + 1 < argc) {
                config.inFile = argv[++i];
            } else {
                errorMessage = "Error: after the flag --in, the path to the image must be specifed";
                return false;
            }
        } else if (arg == "--out") {
            if (i + 1 < argc) {
                config.outFile = argv[++i];
            } else {
                errorMessage = "Error: after the flag --out, the path to the output image must be specifed";
                return false;
            }
        } else if (arg == "--key") {
            if (i + 1 < argc) {
                config.passphrase = argv[++i];
            } else {
                errorMessage = "Error: key was missied";
                return false;
            }
        } else if(arg == "--help") {
            errorMessage = "Action: The user requsted instuction";
            return false;
        } else {
            errorMessage = "Error: unknown parametr";
            return false;
        }
    }

    if (config.modeCrypt == config.modeEncrypt) {
        // Должен быть выбран ровно один режим
        errorMessage = "Choose only one mode: either --crypt or --encrypt";
        return false;
    }

    // Проверка обязательных параметров
    if (config.inFile.empty()) {
        errorMessage = "The parametr --in [input image path] is required";
        return false;
    }
    if (config.modeCrypt && config.textMessage.empty()) {
            errorMessage = "In --crypt mode you have to input text that will be hidden in the picture";
            return false;
        }

    if (config.modeEncrypt && config.passphrase.empty()) {
        errorMessage = "In --encrypt the --key is required argument";
        return false;
    }

    return true;
}

void CliParser::generatePassphrase(std::string& passphrase){
    // Предлагаем сгенерировать случайный пароль
    std::vector<uint8_t> randomKey = Utils::getRandomBytes(16); // например, 16 байт
    passphrase = Utils::bytesToHex(randomKey);
    std::cout << "The key was not specifed. A new key was generated: " << passphrase << "\n"
                << "Store it to encrypt text in the future. If you don't agree, terminate the program\n";
    std::cout << "Press Enter to continue...";
    std::cin.ignore();
}

std::string CliParser::promtSaveFileInCurrentDirectory(const std::string&& fileName){
    std::cout << "The output file path was not specitisfied. Do you agree to use the current directory?\n"
              << "\nIf you agree, enter y: ";
    char choice;
    std::cin >> choice;
    if(choice == 'y' || choice == 'Y'){
        return std::filesystem::current_path().string() + "\\" + fileName;
    }
    LOG_ERROR("The output image path was not specified. User didn't allow to save the output image in the current directory");
    exit(EXIT_FAILURE);
}


void CliParser::validateOutputPath(std::string& outputPath){
    LOG_INFO("The output path before validation {}", outputPath);

    std::filesystem::path output(outputPath);
    std::filesystem::path parrentPath = output.parent_path();
    std::filesystem::path fileName = output.filename();

    if(parrentPath.empty() || !std::filesystem::is_directory(parrentPath)){
        if(fileName.empty()){
        fileName = "output.bmp";
        } else if(fileName.extension().empty()){
            fileName += ".bmp";
        }
        outputPath = promtSaveFileInCurrentDirectory(fileName.string());
    } else if(fileName.extension().empty()) {
        outputPath += ".bmp";
        output = std::filesystem::path(outputPath);
    }
    if(std::filesystem::exists(output)){
        int count = 1;
        do{
            output = output.parent_path() / (output.stem().string() + "(" + std::to_string(count) + ")" + output.extension().string());
        }while(std::filesystem::exists(output));
        outputPath = output.string();
    }

    LOG_INFO("The output path after validation {}", outputPath);
}