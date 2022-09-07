#ifndef __RENDER_API__
#define __RENDER_API__

#include <memory>


namespace Engine {
    namespace Render {
        enum Backend {
            OPENGL,
        };


        class Render {
        private:
            static std::unique_ptr<Render> singleton;
            Backend BackendAPI = Backend::OPENGL;

        public:
            static std::unique_ptr<Render>& GetInstance();
            Backend GetBackendAPI();
            void SetBackendAPI(Backend);
        };
    }
}

#endif
