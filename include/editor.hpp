#ifndef __EDITOR_HPP__
#define __EDITOR_HPP__

#include <memory>
#include <vector>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "app.hpp"
#include "meta.hpp"
#include "tools/logger.hpp"
#include "core/window/base.hpp"
#include "core/window/glfw.hpp"
#include "core/render/api.hpp"
#include "core/scene/scene.hpp"
#include "core/scene/ecs/entity.hpp"

using namespace Tool::Logger;


namespace Engine {
    class EngineApplication;


    namespace Editor {
        class EngineEditor {
        private:
            EngineApplication *app = nullptr;

            void SelectEntity(std::shared_ptr<Scene::Entity>);
            void DrawSceneGraph(std::shared_ptr<Scene::Node>);

        public:
            std::unique_ptr<Logger> logger = std::make_unique<Logger>("editor");

            void Startup(EngineApplication *);
            void Update();
            void Shutdown();
        };
    }
}

#endif
