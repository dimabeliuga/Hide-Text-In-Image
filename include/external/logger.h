#ifndef LOGGER_H
#define LOGGER_H


#ifdef SPDLOG_ACTIVE_LEVEL 

#include <iostream>
#include "spdlog/spdlog.h"
#include "spdlog/async.h"
#include "spdlog/sinks/udp_sink.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/rotating_file_sink.h"

class LoggerConfigBuilder {
private:
    struct LoggerConfig {
        unsigned int MAX_LOG_FILE_SIZE = 1048576 * 5; //5megabytes
        unsigned int MAX_NUMBER_OF_LOG_FILES = 1;
        std::string log_file_path{"../../logs/hide_text_in_image_logs.txt"};
        std::string log_pattern{"[%H:%M:%S : %t] [%^%l%$] %v"};
        std::string multi_logger_information{"multi logger"};
        spdlog::level::level_enum console_log_level = spdlog::level::warn;
        spdlog::level::level_enum file_log_level = spdlog::level::debug;
    };

    LoggerConfig config;

public:
    LoggerConfigBuilder() = default;

    LoggerConfigBuilder& setMaxLogFileSize(unsigned int size) {
        config.MAX_LOG_FILE_SIZE = size;
        return *this;
    }

    LoggerConfigBuilder& setMaxNumberOfLogFiles(unsigned int num) {
        config.MAX_NUMBER_OF_LOG_FILES = num;
        return *this;
    }

    LoggerConfigBuilder& setLogFilePath(const std::string& path) {
        config.log_file_path = path;
        return *this;
    }

    LoggerConfigBuilder& setLogPattern(const std::string& pattern) {
        config.log_pattern = pattern;
        return *this;
    }

    LoggerConfigBuilder& setLoggerInfo(const std::string& info) {
        config.multi_logger_information = info;
        return *this;
    }

    LoggerConfigBuilder& setConsoleLogLevel(spdlog::level::level_enum level) {
        config.console_log_level = level;
        return *this;
    }

    LoggerConfigBuilder& setFileLogLevel(spdlog::level::level_enum level) {
        config.file_log_level = level;
        return *this;
    }

    void setupLogger() const {
        try {
            auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
            console_sink->set_level(config.console_log_level);
            
            auto file_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
                config.log_file_path,
                config.MAX_LOG_FILE_SIZE,
                config.MAX_NUMBER_OF_LOG_FILES);
            
            file_sink->set_level(config.file_log_level);

            if (!console_sink || !file_sink) {
                std::cerr << "Failed to create logger sinks";
            }

            auto logger = std::make_shared<spdlog::logger>(
                config.multi_logger_information,
                spdlog::sinks_init_list{file_sink, console_sink}
            );

            spdlog::set_default_logger(logger);
            spdlog::set_pattern(config.log_pattern);
            spdlog::set_level(spdlog::level::debug);
        } catch (const spdlog::spdlog_ex& ex) {
            std::cerr << "\nLogger initialization failed: " << ex.what();
        } catch (const std::exception& ex) {
            std::cerr << "\nLogger initialization failed: " << ex.what();
        }
    }
};

inline void setup_logger() {
    LoggerConfigBuilder().setupLogger();  // Use default configuration
}

#define LOG_DEBUG(msg, ...)    do{spdlog::debug    (msg, ##__VA_ARGS__);}while(0)
#define LOG_INFO(msg, ...)     do{spdlog::info     (msg, ##__VA_ARGS__);}while(0)
#define LOG_WARN(msg, ...)     do{spdlog::warn     (msg, ##__VA_ARGS__);}while(0)
#define LOG_ERROR(msg, ...)    do{spdlog::error    (msg, ##__VA_ARGS__);}while(0)
#define LOG_CRITICAL(msg, ...) do{spdlog::critical (msg, ##__VA_ARGS__);}while(0)

#else
void setup_logger() {}
#define LOG_DEBUG(msg, ...)    do{}while(0)
#define LOG_INFO(msg, ...)     do{}while(0)
#define LOG_WARN(msg,...)      do{}while(0)
#define LOG_ERROR(msg, ...)    do{}while(0)
#define LOG_CRITICAL(msg, ...) do{}while(0)
#endif

#endif


/*
LoggerConfigBuilder()
    .setMaxLogFileSize(1048576 * 20)
    .setMaxNumberOfLogFiles(5)
    .setLogFilePath("custom/path/to/log.txt")
    .setLogPattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] %v")
    .setLoggerInfo("custom_logger")
    .setConsoleLogLevel(spdlog::level::debug)
    .setFileLogLevel(spdlog::level::trace)
    .setupLogger();
    
*/