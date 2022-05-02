#include "app.hpp"

using namespace Engine;


Application::Application() {
    logger->trace(std::string("constructor"));

    window = Window::WindowProvider::GetInstance();
    editor = new Editor::Editor();
    scene = new Scene::Scene();
}


Application::~Application() {
    logger->trace(std::string("destructor"));

    delete editor;
    delete window;
    delete scene;
}
