cmake_minimum_required(VERSION 3.25)
project(SteganoEncrypt)

set(CMAKE_CXX_STANDARD 17)  # Update to C++17
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add compile definitions
add_definitions(-DSPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_TRACE)
add_definitions(-DUSE_LOGGER)

# Find packages properly
find_package(OpenSSL REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)

# Include directories
include_directories(
    ${PROJECT_SOURCE_DIR}/include 
    ${PROJECT_SOURCE_DIR}/external
    ${PROJECT_SOURCE_DIR}/encryption
)

# Source files
set(SOURCES
    src/main.cpp
    src/image_handler.cpp
    src/stegano.cpp
    src/CliParser.cpp
    src/encryption/utils.cpp
    src/encryption/encryption.cpp
    src/encryption/key_derivation.cpp
    src/encryption/data_conversion.cpp
    src/encryption/decryption.cpp
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Link libraries properly
target_link_libraries(${PROJECT_NAME} PRIVATE
    OpenSSL::SSL
    OpenSSL::Crypto
    spdlog::spdlog
    fmt::fmt
)
