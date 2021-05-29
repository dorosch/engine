#include "tools/logger.hpp"


namespace Engine {
    namespace Logger {
        Logger::Logger(const char *name) {
            this->_level = Level::info;
            this->_logger = spdlog::stdout_logger_mt(name);
        }


        Level Logger::GetLevel() {
            return this->_level;
        }


        void Logger::SetLevel(Level level) {
            switch (level) {
                case Level::trace:
                    this->_level = Level::trace;
                    this->_logger->set_level(spdlog::level::trace);
                    break;
                case Level::debug:
                    this->_level = Level::debug;
                    this->_logger->set_level(spdlog::level::debug);
                    break;
                case Level::info:
                    this->_level = Level::info;
                    this->_logger->set_level(spdlog::level::info);
                    break;
                case Level::warning:
                    this->_level = Level::warning;
                    this->_logger->set_level(spdlog::level::warn);
                    break;
                case Level::error:
                    this->_level = Level::error;
                    this->_logger->set_level(spdlog::level::err);
                    break;
                case Level::critical:
                    this->_level = Level::critical;
                    this->_logger->set_level(spdlog::level::critical);
                    break;
            }
        }


        void Logger::trace(const char *message) {
            this->_logger->trace(message);
        }


        void Logger::debug(const char *message) {
            this->_logger->debug(message);
        }


        void Logger::info(const char *message) {
            this->_logger->info(message);
        }


        void Logger::warning(const char *message) {
            this->_logger->warn(message);
        }


        void Logger::error(const char *message) {
            this->_logger->error(message);
        }


        void Logger::critical(const char *message) {
            this->_logger->critical(message);
        }
    }
}
