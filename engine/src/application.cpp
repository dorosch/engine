#include "application.hpp"


namespace Engine {
    void Application::Run() {
        logger->debug("run");

        logger->info(fmt::format("Engine version: {}", ENGINE_VERSION).c_str());

        while (true);
    }
}
