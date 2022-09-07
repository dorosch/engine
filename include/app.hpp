#ifndef __APPLICATION__
#define __APPLICATION__

#include <memory>

#include "editor.hpp"
#include "tools/logger.hpp"
#include "core/scene/scene.hpp"
#include "core/window/base.hpp"

using namespace Tool::Logger;


namespace Engine {
    class EngineApplication {
        /**
         * Custom engine application class.
         * 
         * When creating an application using the engine, the entry 
         * point is a class inherited from this one. Also, the custom 
         * application needs to override three methods:
         *     Startup()
         *     Update()
         *     Shutdown()
         * 
         * A user application should not override a constructor or 
         * destructor without calling the parent construct or destructor. 
         */

    public:
        std::unique_ptr<Logger> logger = std::make_unique<Logger>("app");

        Window::Provider provider = Window::Provider::GLFW;
        Window::WindowProvider *window = nullptr;
        Editor::EngineEditor *editor = nullptr;
        Scene::Scene *scene = nullptr;
        std::unique_ptr<Render::Render> &render = Render::Render::GetInstance();

        EngineApplication();
        virtual ~EngineApplication();
        virtual void Startup() = 0;
        virtual void Run() = 0;
        virtual void Update() = 0;
        virtual void Shutdown() = 0;
    };
}

#endif
