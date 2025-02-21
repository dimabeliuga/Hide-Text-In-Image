/**
 * @file CliParser.h
 * @author Dmytro Beliuha 
 * @brief File with only one structure that is used to parse command line arguments
 * @version 0.1
 * @date 2025-02-19
 * @copyright Copyright (c) 2025
 */

#ifndef CLI_PARSER_H
#define CLI_PARSER_H

#include "CliConfig.h"
#include <string>

/**
 * @brief structure with function for parsing command line
 * 
 */
struct CliParser
{
    // function for extracting command line arguments
    static CliConfig& parse(int argc, char** argv);

private:
    static std::string errorMessage;
    
    // function outputs error message
    static void getErrorMessage();

    // funtion that will output instuction how to use programme
    static void printUsage();

    // helper function that will only extract arguments
    static bool extractCommandLineArguments(int argc, char** argv, CliConfig& config);

    // function that will generate key, if the user doesn't specify
    static void generatePassphrase(std::string& passphrase);

    // if out file path was not specified, ask if the user allows to save out image in the current directory
    static std::string promtSaveFileInCurrentDirectory(const std::string&& fileName);

    // function to check whether the output path is valide or not
    static void validateOutputPath(std::string& outputPath);
};


#endif