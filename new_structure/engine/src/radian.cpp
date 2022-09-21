#include "radian.hpp"

using namespace Radian;


Application::Application() {}


Application::~Application() {}


void Engine::run(std::unique_ptr<Application> app) {
    _app = std::move(app);

    _app->startup();
    _app->run();
    _app->shutdown();
}
