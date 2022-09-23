#include "tools/logger.hpp"

using namespace Radian::Tool;


Logger::Logger(const std::string &name) {
    try {
        _logger = spdlog::stdout_color_mt(name);
    }
    catch (const spdlog::spdlog_ex &ex) {
        _logger = spdlog::get(name);
    }
}


Logger::Level Logger::getLevel() {
    return static_cast<Logger::Level>(_logger->level());
}


void Logger::setLevel(Level level) {
    _logger->set_level(static_cast<spdlog::level::level_enum>(level));
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
