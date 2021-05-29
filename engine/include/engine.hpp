#ifndef __ENGINE_HPP__
#define __ENGINE_HPP__

#include "tools/logger.hpp"
#include "core/manager/window.hpp"


namespace Engine{
    class EngineApplication {
    public:
        EngineApplication();

        virtual ~EngineApplication();

        Tool::Logger::Logger *logger;

        Manager::WindowManager *window;

        virtual void Init() = 0;

        virtual void Update() = 0;

        virtual void Shutdown() = 0;
    };


    class Engine {
    private:
        EngineApplication *application;

    public:
        Engine(EngineApplication *);

        ~Engine();

        Tool::Logger::Logger *logger;

        void Run();
    };
}


#endif
