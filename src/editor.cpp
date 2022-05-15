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

            ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;

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
                if (ImGui::TreeNode(app->scene->root->name.c_str())) {
                    for (Engine::Scene::Entity *node : app->scene->root->children) {
                        DrawSceneGraph(node, node->children);
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
        }


        void EngineEditor::Shutdown() {
            logger->trace(std::string("Shutdown"));

            ImGui_ImplOpenGL3_Shutdown();
            ImGui_ImplGlfw_Shutdown();
            ImGui::DestroyContext();
        }


        void EngineEditor::DrawSceneGraph(Engine::Scene::Entity *node, std::vector<Engine::Scene::Entity*> children) {
            if (children.empty()) {
                ImGui::TreeNodeEx(node->name.c_str(), ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_SpanFullWidth);

                if (ImGui::IsItemClicked()) {
                    SelectEntity(node);
                }
            }
            else {
                bool open = ImGui::TreeNodeEx(node->name.c_str(), ImGuiTreeNodeFlags_SpanFullWidth);

                if (open) {
                    for (Engine::Scene::Entity *node : children) {
                        DrawSceneGraph(node, node->children);
                    }

                    ImGui::TreePop();
                }
            }
        }


        void EngineEditor::SelectEntity(Engine::Scene::Entity *entity) {
            logger->info(fmt::format("Selected entity: {}", entity->name));
        }
    }
}
