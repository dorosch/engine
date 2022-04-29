#ifndef __LOGGER__
#define __LOGGER__

#include <map>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>


namespace Tool {
    namespace Logger {
        enum Level {
            TRACE,
            DEBUG,
            INFO,
            WARNING,
            ERROR,
            CRITICAL
        };

        class Logger {
        protected:
            Level _level = Level::INFO;
            std::shared_ptr<spdlog::logger> _logger;

        public:
            Logger(const std::string &);

            Level GetLevel();
            void SetLevel(Level);
            void trace(const std::string &);
            void debug(const std::string &);
            void info(const std::string &);
            void warning(const std::string &);
            void error(const std::string &);
            void critical(const std::string &);
        };
    }
}

#endif
