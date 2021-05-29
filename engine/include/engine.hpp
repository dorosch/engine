#ifndef __ENGINE_HPP__
#define __ENGINE_HPP__

#include "tools/logger.hpp"
#include "core/manager/window.hpp"


namespace Engine{
    class EngineApplication {
    public:
        EngineApplication();

        ~EngineApplication();

        Manager::WindowManager *window;

        virtual void Init() = 0;

        virtual void Update() = 0;

        virtual void Shutdown() = 0;
    };


    class Engine {
    private:
        EngineApplication *application;
        Tool::Logger::Logger *logger;

    public:
        Engine(EngineApplication *);

        ~Engine();

        void Run();
    };
}


#endif
