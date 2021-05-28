#ifndef __LOGGER_HPP__
#define __LOGGER_HPP__

#include <memory>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_sinks.h>


namespace Engine {
    namespace Logger {
        enum Level {
            trace,
            debug,
            info,
            warning,
            error,
            critical
        };

        class Logger {
        private:
            Level _level;
            std::shared_ptr<spdlog::logger> _logger;

        public:
            Logger(const char *name) {
                this->_level = Level::info;
                this->_logger = spdlog::stdout_logger_mt(name);
            }

            void SetLevel(Level level) {
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

            Level GetLevel() {
                return this->_level;
            }

            void trace(const char *message) {
                this->_logger->trace(message);
            }

            void debug(const char *message) {
                this->_logger->debug(message);
            }

            void info(const char *message) {
                this->_logger->info(message);
            }

            void warning(const char *message) {
                this->_logger->warn(message);
            }

            void error(const char *message) {
                this->_logger->error(message);
            }

            void critical(const char *message) {
                this->_logger->critical(message);
            }
        };
    }
}


#endif
