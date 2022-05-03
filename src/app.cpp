#include "app.hpp"

using namespace Engine;



EngineApplication::EngineApplication() {
    logger->trace(std::string("constructor"));

    window = Window::WindowProvider::GetInstance();
    editor = new Editor::EngineEditor();
    scene = new Scene::Scene();
}


EngineApplication::~EngineApplication() {
    logger->trace(std::string("destructor"));

    delete editor;
    delete window;
    delete scene;
}
