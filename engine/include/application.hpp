#ifndef __APPLICATION_HPP__
#define __APPLICATION_HPP__

#include <iostream>
#include <fmt/format.h>

#include "core.hpp"
#include "logger.hpp"


namespace Engine {
    class Application {
    private:
        Logger::Logger *logger;

    public:
        Application() {
            logger = new Logger::Logger("engine");

            logger->SetLevel(Logger::Level::trace);

            logger->debug("init");
        }

        ~Application() {
            logger->debug("finish");

            delete logger;
        }

        void Run();
    };
}


#endif
