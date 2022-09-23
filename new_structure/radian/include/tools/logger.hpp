#ifndef __RADIAN_TOOL_LOGGER__
#define __RADIAN_TOOL_LOGGER__

#include <map>
#include <memory>
#include <string>

#include <iostream>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace Radian {
    namespace Tool {
        class Logger {
        private:
            std::shared_ptr<spdlog::logger> _logger;

        public:
            enum Level {
                TRACE,
                DEBUG,
                INFO,
                WARNING,
                ERROR,
                CRITICAL
            };

            Logger(const std::string &);
            Level getLevel();
            void setLevel(Level);
            void trace(const std::string &);
            void debug(const std::string &);
            void info(const std::string &);
            void warning(const std::string &);
            void error(const std::string &);
            void critical(const std::string &);
        };


        static std::map<std::string, std::shared_ptr<Logger>> loggers;


        inline std::shared_ptr<Logger> &getLogger(const std::string &name) {
            auto logger = loggers.find(name);

            if (logger == loggers.end()) {
                loggers.emplace(name, std::make_shared<Logger>(name));

                return loggers[name];
            }

            return logger->second;
        }
    }
}

#endif
