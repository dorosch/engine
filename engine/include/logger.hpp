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
            Logger(const char *);

            Level GetLevel();

            void SetLevel(Level);

            void trace(const char *);

            void debug(const char *);

            void info(const char *);

            void warning(const char *);

            void error(const char *);

            void critical(const char *);
        };
    }
}


#endif
