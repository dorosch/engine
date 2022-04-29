#include "tools/logger.hpp"

using namespace Tool::Logger;


Logger::Logger(const std::string &name) {
    try {
        _logger = spdlog::stdout_color_mt(name);
    }
    catch (const spdlog::spdlog_ex &ex) {
        _logger = spdlog::get(name);
    }
}


Level Logger::GetLevel() {
    return _level;
}


void Logger::SetLevel(Level level) {
    switch (level) {
        case Level::TRACE:
            _level = Level::TRACE;
            _logger->set_level(spdlog::level::trace);
            break;
        case Level::DEBUG:
            _level = Level::DEBUG;
            _logger->set_level(spdlog::level::debug);
            break;
        case Level::INFO:
            _level = Level::INFO;
            _logger->set_level(spdlog::level::info);
            break;
        case Level::WARNING:
            _level = Level::WARNING;
            _logger->set_level(spdlog::level::warn);
            break;
        case Level::ERROR:
            _level = Level::ERROR;
            _logger->set_level(spdlog::level::err);
            break;
        case Level::CRITICAL:
            _level = Level::CRITICAL;
            _logger->set_level(spdlog::level::critical);
            break;
    }
}


void Logger::trace(const std::string &message) {
    _logger->trace(message);
}


void Logger::debug(const std::string &message) {
    _logger->debug(message);
}


void Logger::info(const std::string &message) {
    _logger->info(message);
}


void Logger::warning(const std::string &message) {
    _logger->warn(message);
}


void Logger::error(const std::string &message) {
    _logger->error(message);
}


void Logger::critical(const std::string &message) {
    _logger->critical(message);
}