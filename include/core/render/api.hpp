#ifndef __RENDER_API__
#define __RENDER_API__

#include <memory>

#include <GL/glew.h>

#include "tools/logger.hpp"
#include "core/render/shader/base.hpp"
#include "core/render/shader/opengl.hpp"
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

            std::unique_ptr<ShaderProgram> shader;

            static std::shared_ptr<Render>& GetInstance();
            Backend GetBackendAPI();
            void SetBackendAPI(Backend);
            void Startup();
            void Shutdown();
            void RenderScene(Engine::Scene::Scene *);
        };
    }
}

#endif
