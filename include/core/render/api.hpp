#ifndef __RENDER_API__
#define __RENDER_API__

#include <memory>

#include "tools/logger.hpp"
#include "core/scene/scene.hpp"

using namespace Tool::Logger;


namespace Engine {
    namespace Render {
        enum Backend {
            OPENGL,
        };


        class Render {
        private:
            static std::shared_ptr<Render> singleton;
            Backend BackendAPI = Backend::OPENGL;

        public:
            std::unique_ptr<Logger> logger = std::make_unique<Logger>("render");

            static std::shared_ptr<Render>& GetInstance();
            Backend GetBackendAPI();
            void SetBackendAPI(Backend);
            void RenderScene(Engine::Scene::Scene *);
        };
    }
}

#endif
