cmake_minimum_required(VERSION 3.25)
project(SteganoEncrypt)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Включаем пути для заголовочных файлов
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/external/)

# Добавляем исходные файлы
set(SOURCES
    src/main.cpp
    src/encryption.cpp
    src/key_derivation.cpp
    src/image_handler.cpp
    src/stegano.cpp
    src/utils.cpp
)

# Добавляем исполняемый файл
add_executable(${PROJECT_NAME} ${SOURCES})

# Линкуем с OpenSSL (проверьте, что OpenSSL установлен и CMake может его найти)
find_package(OpenSSL REQUIRED)
if(OPENSSL_FOUND)
    target_include_directories(${PROJECT_NAME} PRIVATE ${OPENSSL_INCLUDE_DIR})
    target_link_libraries(${PROJECT_NAME} ${OPENSSL_LIBRARIES})
endif()
