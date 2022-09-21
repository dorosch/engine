#ifndef __RADIAN__
#define __RADIAN__

#include <memory>


namespace Radian {
    class Application {
    public:
        Application();
        virtual ~Application();
        virtual void startup() = 0;
        virtual void run() = 0;
        virtual void shutdown() = 0 ;
    };


    class Engine {
    private:
        std::unique_ptr<Application> _app;

    public:
        void run(std::unique_ptr<Application>);
    };

    static std::unique_ptr<Radian::Engine> engine = std::make_unique<Radian::Engine>();
}

#endif
