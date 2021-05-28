#ifndef __APPLICATION_HPP__
#define __APPLICATION_HPP__

#include <iostream>
#include <fmt/format.h>

#include "meta.hpp"
#include "logger.hpp"


namespace Engine {
    class Application {
    private:
        Logger::Logger *logger;

    public:
        Application() {
            logger = new Logger::Logger("engine");

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
