#include "app.hpp"
#include "editor.hpp"


float position[] = {0.0, 0.0, 0.0};
float color[4] = {1.0f, 1.0f, 1.0f, 1.0f};


namespace Engine {
    namespace Editor {
        void EngineEditor::Startup(EngineApplication *app) {
            this->app = app;

            logger->trace(std::string("Init"));

            IMGUI_CHECKVERSION();
            ImGui::CreateContext();
            ImGuiIO& io = ImGui::GetIO(); (void)io;

            ImGui::StyleColorsDark();

            // Setup Platform/Renderer backends
            switch (this->app->provider) {
                case Window::Provider::GLFW:
                    ImGui_ImplGlfw_InitForOpenGL(
                        static_cast<Engine::Window::GLFWWindowProvider*>(this->app->window)->object, true
                    );
                    break;
                default:
                    throw std::logic_error("Undefined WindowProvider");
            }

            switch (Engine::Render::GetBackendAPI()) {
                case Render::Backend::OPENGL:
                    ImGui_ImplOpenGL3_Init(GLSL_VERSION);
                    break;
                default:
                    throw std::logic_error("Unsopported backend API");
            }
        }


        void EngineEditor::Update() {
            logger->trace(std::string("Update"));

            static bool closed = false;
            static bool wireframe = false;

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            ImGui::Begin("Main window", &closed, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse);
                ImGui::Checkbox("Wireframe", &wireframe);

                ImGui::Text(
                    "Application average %.3f ms/frame (%.1f FPS)",
                    1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate
                );
            ImGui::End();

            ImGui::Begin("Triangle Position/Color", &closed, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse);
                ImGui::SliderFloat3("Position", position, -1.0, 1.0);
                ImGui::ColorEdit3("Color", color);
            ImGui::End();

            ImGui::Begin("Scene graph", &closed);
                if (ImGui::TreeNode("root")) {
                    bool open = ImGui::TreeNodeEx("test 1", ImGuiTreeNodeFlags_SpanFullWidth);
                    if (open) {
                        ImGui::TreePop();
                    }
                    ImGui::TreeNodeEx("test", ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_SpanFullWidth);
                    if (ImGui::IsItemClicked()) {

                    }
                    ImGui::TreePop();
                }
            ImGui::End();

            if (wireframe) {
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            }
            else {
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            }

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            // int display_w, display_h;
            // glfwGetFramebufferSize(this->app->window->object, &display_w, &display_h);
            // glViewport(0, 0, display_w, display_h);
        }


        void EngineEditor::Shutdown() {
            logger->trace(std::string("Shutdown"));

            ImGui_ImplOpenGL3_Shutdown();
            ImGui_ImplGlfw_Shutdown();
            ImGui::DestroyContext();
        }


        void EngineEditor::SelectEntity(Engine::Scene::Entity *entity) {
            logger->info(fmt::format("Selected entity: {}", entity->name));
        }
    }
}
