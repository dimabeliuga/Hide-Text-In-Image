/**
 * @file CliParser.h
 * @author Dmytro Beliuha
 * @brief Header file containing a structure for parsing command-line arguments.
 * @version 0.1
 * @date 2025-02-19
 * @copyright Copyright (c) 2025
 */

 #ifndef CLI_PARSER_H
 #define CLI_PARSER_H
 
 #include "CliConfig.h"
 #include <string>
 
 /**
  * @brief Structure responsible for parsing command-line arguments.
  * 
  * Provides functionality for extracting arguments, displaying usage instructions, 
  * generating passphrases, and validating output paths.
  */
 struct CliParser {
     /**
      * @brief Parses command-line arguments and stores them in CliConfig.
      * 
      * This function processes input arguments, sets program modes (encryption or decryption),
      * validates input/output paths, and ensures correct configuration.
      * 
      * @param argc Argument count.
      * @param argv Argument vector.
      * @return CliConfig& Reference to the parsed configuration.
      */
     static CliConfig& parse(int argc, char** argv);
 
 private:
     static std::string errorMessage; ///< Stores the error message if parsing fails.
 
     /**
      * @brief Outputs an error message.
      * 
      * Displays an error message when incorrect arguments are provided.
      */
     static void getErrorMessage();
 
     /**
      * @brief Prints instructions on how to use the program.
      * 
      * Provides guidance on valid command-line arguments and expected input format.
      */
     static void printUsage();
 
     /**
      * @brief Extracts command-line arguments and fills the CliConfig structure.
      * 
      * @param argc Argument count.
      * @param argv Argument vector.
      * @param config Reference to the configuration structure.
      * @return true If arguments were successfully extracted.
      * @return false If extraction failed due to invalid input.
      */
     static bool extractCommandLineArguments(int argc, char** argv, CliConfig& config);
 
     /**
      * @brief Generates a random passphrase if the user does not specify one.
      * 
      * The generated passphrase can be used for encryption/decryption.
      * 
      * @param passphrase Reference to the passphrase string, which will be set if empty.
      */
     static void generatePassphrase(std::string& passphrase);
 
     /**
      * @brief Asks the user for permission to save the output image in the current directory.
      * 
      * If no output path is specified, this function prompts the user and suggests a default filename.
      * 
      * @param fileName The default filename suggested.
      * @return std::string The final output path chosen by the user.
      */
     static std::string promtSaveFileInCurrentDirectory(const std::string&& fileName);
 
     /**
      * @brief Validates the specified output file path.
      * 
      * Ensures that the output directory exists and that the filename is correctly formatted.
      * If necessary, prompts the user to modify the output path.
      * 
      * @param outputPath Reference to the output file path string.
      */
     static void validateOutputPath(std::string& outputPath);
 };
 
 #endif // CLI_PARSER_H