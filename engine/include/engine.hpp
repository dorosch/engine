#ifndef __ENGINE_HPP__
#define __ENGINE_HPP__

#include <memory>

#include "tools/logger.hpp"
#include "editor/editor.hpp"
#include "core/event/manager.hpp"
#include "core/window/manager.hpp"

using namespace Tool::Logger;


namespace Engine {
    class EngineEditor;


    class EngineApplication {
        /**
         * Custom engine application class.
         * 
         * When creating an application using the engine, the entry 
         * point is a class inherited from this one. Also, the custom 
         * application needs to override three methods:
         *     Init()
         *     Update()
         *     Shutdown()
         * 
         * A user application should not override a constructor or 
         * destructor without calling the parent construct or destructor. 
         */

    public:
        std::unique_ptr<Logger> logger = std::make_unique<Logger>("app");

        Manager::WindowManager *windowManager = nullptr;
        Manager::EventManager *eventManager = nullptr;
        Editor::EngineEditor *editor = nullptr;


        EngineApplication();
        virtual ~EngineApplication();
        virtual void Init() = 0;
        virtual void Update() = 0;
        virtual void Shutdown() = 0;
    };


    class Engine {
        /**
         * Main engine class.
         * 
         * When used, the engine must have a custom user application. 
         * Further work of the engine comes down to starting 
         * main loop and updating managers and the user application.
         */

    private:
        EngineApplication *app;

    public:
        std::unique_ptr<Logger> logger = std::make_unique<Logger>("engine");

        Engine(EngineApplication *);
        void Init();
        void Run();
    };
}


#endif
