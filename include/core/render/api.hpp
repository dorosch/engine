#ifndef __RENDER_API__
#define __RENDER_API__

#include <memory>

#include <GL/glew.h>

#include "tools/logger.hpp"
#include "core/render/shader/base.hpp"
#include "core/render/shader/opengl.hpp"
#include "core/graphics/lighting/base.hpp"
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
            ShaderProgram *shader;
            std::unique_ptr<ShaderProgram> defaultShader;
            std::unique_ptr<ShaderProgram> lightingShader;
            std::unique_ptr<ShaderProgram> materialShader;

            static std::shared_ptr<Render>& GetInstance();
            Backend GetBackendAPI();
            void SetBackendAPI(Backend);
            void Startup();
            void Shutdown();
            void RenderScene(Engine::Scene::Scene *, glm::mat4, glm::mat4, glm::vec3);
            void RenderObject(Engine::Object *, glm::mat4, glm::mat4, glm::vec3, Graphics::Lighting::Light *);
        };
    }
}

#endif
