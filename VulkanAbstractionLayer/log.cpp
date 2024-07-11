//
// Created by win111 on 2024/7/9.
//
#include "log.h"

std::shared_ptr<spdlog::logger> Logger::s_logger;
void Logger::init() {
    spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%l]  %v%$");
    s_logger = spdlog::stdout_color_mt("logger");
    s_logger->set_level(spdlog::level::trace);

}

void Logger::set_printer_mode() { s_logger->set_pattern("%v%$"); }

void Logger::set_default_mode() { s_logger->set_pattern("%n: %v%$"); }