//
// Created by win111 on 2024/7/9.
//

#ifndef VULKANABSTRACTIONLAYER_LOG_H
#define VULKANABSTRACTIONLAYER_LOG_H
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <memory>
#include <stdexcept>


#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_DEBUG
class Logger{

public:
    static void init();
    static void set_printer_mode();
    static void set_default_mode();
    inline static std::shared_ptr<spdlog::logger>& get_logger() { return s_logger; }

private:
    static std::shared_ptr<spdlog::logger> s_logger;
};

#define LOG_TRACE(...) Logger::get_logger()->trace(__VA_ARGS__)
#ifdef _DEBUG
#define LOG_INFO(...) Logger::get_logger()->info(__VA_ARGS__)
#else
#define LOG_INFO(...)
#define VEX_INFO(...)
#endif
#define LOG_WARN(...) Logger::get_logger()->warn(__VA_ARGS__)

#define LOG_ERROR(...)  \
{   \
    Logger::get_logger()->error(__VA_ARGS__);                       \
    throw std::runtime_error("Error: " + std::string(__VA_ARGS__)); \
}

#endif //VULKANABSTRACTIONLAYER_LOG_H
