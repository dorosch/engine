#include <iostream>

#include <core.hpp>


class Sandbox : public Engine::Application {
private:
    Engine::Logger::Logger *logger;

public:
    Sandbox() {
        logger = new Engine::Logger::Logger("sandbox");

        logger->SetLevel(Engine::Logger::Level::trace);

        logger->debug("init");
    }

    ~Sandbox() {
        logger->debug("finish");

        delete logger;
    }
};


int main() {
    Sandbox *application = new Sandbox();

    try {
        application->Run();
    } catch (const std::exception& error) {
        std::cerr << error.what() << std::endl;

        delete application;

        return EXIT_FAILURE;
    }

    delete application;

    return EXIT_SUCCESS;
}
