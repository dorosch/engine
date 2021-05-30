#ifndef __WINDOW_PROVIDER_HPP__
#define __WINDOW_PROVIDER_HPP__

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
            const char *title;
        };


        class WindowProvider {
        public:
            Settings settings = {
                800, 600, false, false, "Application"
                };

            virtual void Init() = 0;
            virtual void Create() = 0;
            virtual void Update() = 0;
            virtual void Shutdown() = 0;
            virtual bool IsOpen() = 0;
        };
    }
}

#endif
