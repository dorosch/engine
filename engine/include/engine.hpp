#ifndef __ENGINE_HPP__
#define __ENGINE_HPP__

#include "logger.hpp"


namespace Engine{
    class EngineApplication {
    public:
        virtual void Run() = 0;

        virtual void Update() = 0;

        virtual void Shutdown() = 0;
    };


    class Engine {
    private:
        EngineApplication *application;
        Logger::Logger *logger;

    public:
        Engine(EngineApplication *);

        ~Engine();

        void Run();
    };
}


#endif
