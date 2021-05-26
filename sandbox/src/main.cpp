#include "core.hpp"


class Sandbox : public Engine::Application {};


int main() {
    Sandbox *application = new Sandbox();

    try {
        application->Run();
    } catch (const std::exception& error) {
        delete application;

        std::cerr << error.what() << std::endl;

        return EXIT_FAILURE;
    }

    delete application;

    return EXIT_SUCCESS;
}
