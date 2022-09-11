#ifndef __ENGINE__
#define __ENGINE__

#include <memory>
#include <fmt/format.h>

#define GLEW_STATIC
#define GLM_FORCE_RADIANS
#include <GL/glew.h>

#include "app.hpp"
#include "meta.hpp"
#include "tools/logger.hpp"
#include "core/window/base.hpp"
#include "core/window/glfw.hpp"
#include "core/render/api.hpp"

using namespace Tool::Logger;


namespace Engine {
    class Engine {
        /**
         * Main engine class.
         * 
         * When used, the engine must have a custom user application. 
         * Further work of the engine comes down to starting 
         * main loop and updating managers and the user application.
         */

    private:
        EngineApplication *app;

    public:
        std::unique_ptr<Logger> logger = std::make_unique<Logger>("engine");

        Engine(EngineApplication *application) {
            logger->trace(std::string("constructor"));

            this->app = application;
        }

        void Startup() {
            /**
             * Engine initialization.
             * 
             * At this stage, the engine initializes the user application 
             * and all managers. Also at this stage, it is possible to check 
             * for the initialization of managers. 
             */

            // Initialize all managers and then initialize the application
            // since the application can change the settings of managers.
            app->window->Startup();
            app->Startup();

            logger->info(fmt::format("engine version: {}", ENGINE_VERSION));
            logger->info(fmt::format("glsl version: {}", GLSL_VERSION));

            switch (Render::Render::GetInstance()->GetBackendAPI()) {
                case Render::Backend::OPENGL:
                    logger->info("backend API: OpenGL");
                    break;
                default:
                    logger->critical("unknown render backend API");
            }
        }
        
        void Run() {
            /**
             * Main engine loop.
             * 
             * As long as the application window is open, engine 
             * continues updating user application and all managers. 
             */

            logger->trace(std::string("run"));

            app->window->Create();
            app->editor->Startup(app);

            // TODO: Move all gl* functions to the render backend
            glewExperimental = GL_TRUE;
            glewInit();

            glEnable(GL_DEPTH_TEST);
            glEnable(GL_MULTISAMPLE);

            app->render->Startup();
            app->scene->startup();
            app->Run();

            while (app->window->IsOpen()) {
                glfwPollEvents();

                glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                app->Update();
                app->editor->Update();
                // TODO: Uncomment after move camera as part of scene
                // app->render->RenderScene(app->scene);
                app->window->Update();
            }

            logger->info("shutdown...");

            app->Shutdown();
            app->render->Shutdown();
            app->editor->Shutdown();
            app->window->Shutdown();
        }
    };
}

#endif
