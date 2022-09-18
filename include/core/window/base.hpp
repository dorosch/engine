#ifndef __WINDOW_BASE__
#define __WINDOW_BASE__

#include <string>

#include "tools/logger.hpp"


namespace Engine {
    namespace Window {
        enum Provider {
            GLFW,
        };


        struct Settings {
            int width;
            int height;
            bool fullScreen;
            bool canResize;
            std::string title;
        };


        class WindowProvider {
        public:
            void *object = nullptr;
            Provider provider;
            Settings settings = {
                800, 600, false, false, std::string("Application")
            };
            std::unique_ptr<Tool::Logger::Logger> logger = 
                std::make_unique<Tool::Logger::Logger>("window");

            // TODO: Pass to the GetInstance the application instance for define window backend
            static WindowProvider *GetInstance();
            virtual ~WindowProvider() {}
            virtual void Startup() = 0;
            virtual void Create() = 0;
            virtual void Update() = 0;
            virtual void Shutdown() = 0;
            virtual bool IsOpen() = 0;
        };
    }
}

#endif
