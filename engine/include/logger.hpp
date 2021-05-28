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
            std::shared_ptr<spdlog::logger> _logger;

        public:
            Logger(const char *name) {
                this->_logger = spdlog::stdout_logger_mt(name);
            }

            void SetLevel(Level level) {
                switch (level) {
                    case Level::trace:
                        this->_logger->set_level(spdlog::level::trace);
                        break;
                    case Level::debug:
                        this->_logger->set_level(spdlog::level::debug);
                        break;
                    case Level::info:
                        this->_logger->set_level(spdlog::level::info);
                        break;
                    case Level::warning:
                        this->_logger->set_level(spdlog::level::warn);
                        break;
                    case Level::error:
                        this->_logger->set_level(spdlog::level::err);
                        break;
                    case Level::critical:
                        this->_logger->set_level(spdlog::level::critical);
                        break;
                }
            }

            template<typename ...Args>
            void trace(Args&&... args) {
                this->_logger->trace(args...);
            }

            template<typename ...Args>
            void debug(Args&&... args) {
                this->_logger->debug(args...);
            }

            template<typename ...Args>
            void info(Args&&... args) {
                this->_logger->info(args...);
            }

            template<typename ...Args>
            void warning(Args&&... args) {
                this->_logger->warn(args...);
            }

            template<typename ...Args>
            void error(Args&&... args) {
                this->_logger->error(args...);
            }

            template<typename ...Args>
            void critical(Args&&... args) {
                this->_logger->critical(args...);
            }
        };
    }
}


#endif
