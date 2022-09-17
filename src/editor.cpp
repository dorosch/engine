#include "app.hpp"
#include "editor.hpp"


std::shared_ptr<Engine::Object> selectedEntity;


namespace Engine {
    namespace Editor {
        void EngineEditor::Startup(EngineApplication *app) {
            this->app = app;

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

            switch (Engine::Render::Render::GetInstance()->GetBackendAPI()) {
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

            ImGui::Begin("Scene graph", &closed);
                if (ImGui::BeginPopupContextItem()) {
                    ImGui::Text("Add");
                    ImGui::Separator();

                    if (ImGui::BeginMenu("mesh")) {
                        if (ImGui::MenuItem("Plane")) {
                            std::shared_ptr<Engine::Geometry::Plane> plane = std::make_shared<Engine::Geometry::Plane>();
                            app->scene->root->entities.push_back(plane);
                        }
                        if (ImGui::MenuItem("Cube")) {
                            std::shared_ptr<Engine::Geometry::Cube> cube = std::make_shared<Engine::Geometry::Cube>();
                            app->scene->root->entities.push_back(cube);
                        }
                        ImGui::EndMenu();
                    }
                    ImGui::EndPopup();
                }

                if (ImGui::TreeNode(app->scene->root->name.c_str())) {
                    for (std::shared_ptr<Object> entity : app->scene->root->entities) {
                        ImGui::TreeNodeEx(entity->name.c_str(), ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_SpanFullWidth);

                        if (ImGui::IsItemClicked()) {
                            SelectEntity(entity);
                        }
                    }

                    for (std::shared_ptr<Scene::Node> node : app->scene->root->children) {
                        DrawSceneGraph(node);
                    }

                    ImGui::TreePop();
                }

                if (ImGui::TreeNode("Lighting")) {
                    for (std::shared_ptr<Graphics::Lighting::Light> light : app->scene->lighting) {
                        ImGui::TreeNodeEx(light->name.c_str(), ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_SpanFullWidth);

                        if (ImGui::IsItemClicked()) {
                            SelectEntity(light);
                        }
                    }

                    ImGui::TreePop();
                }

                if (ImGui::TreeNode("Cameras")) {
                    for (std::shared_ptr<Graphics::Camera::Camera> camera : app->scene->cameras) {
                        ImGui::TreeNodeEx(camera->name.c_str(), ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_SpanFullWidth);

                        if (ImGui::IsItemClicked()) {
                            SelectEntity(camera);
                        }
                    }

                    ImGui::TreePop();
                }
            ImGui::End();

            // Demo window for see examples of widgets
            // ImGui::ShowDemoWindow(&closed);

            ImGui::Begin("Object properties", &closed);
                if (selectedEntity != nullptr) {
                    if (selectedEntity->HasComponent(Ecs::Component::Type::TRANSFORM)) {
                        if (ImGui::CollapsingHeader("Transformation", ImGuiTreeNodeFlags_DefaultOpen)) {
                            ImGui::SliderFloat3("position", &selectedEntity->transform->position[0], -10.0, 10.0);
                            ImGui::SliderFloat3("rotation", &selectedEntity->transform->rotation[0], -1.0, 1.0);
                            ImGui::SliderFloat3("scale", &selectedEntity->transform->scale[0], -1.0, 1.0);
                        }                          
                    }

                    if (selectedEntity->HasComponent(Ecs::Component::Type::MATERIAL)) {
                        if (ImGui::CollapsingHeader("Material", ImGuiTreeNodeFlags_None)) {
                            ImGui::SliderFloat("shininess", &selectedEntity->material->shininess, 0.1, 256.0);
                            ImGui::ColorEdit3("color", &selectedEntity->material->color[0]);
                            ImGui::SliderFloat3("ambient", &selectedEntity->material->ambient[0], 0.0, 1.0);
                            ImGui::SliderFloat3("diffuse", &selectedEntity->material->diffuse[0], 0.0, 1.0);
                            ImGui::SliderFloat3("specular", &selectedEntity->material->specular[0], 0.0, 1.0);
                        }
                    }

                    if (selectedEntity->HasComponent(Ecs::Component::Type::MESH)) {
                        if (ImGui::CollapsingHeader("Mesh", ImGuiTreeNodeFlags_None)) {
                            ImGui::Text("vertices: %ld", selectedEntity->mesh->vertices.size());
                        }
                    }

                    if (selectedEntity->HasComponent(Ecs::Component::Type::LIGHT)) {
                        if (ImGui::CollapsingHeader("Light", ImGuiTreeNodeFlags_None)) {
                            switch (selectedEntity->light->lightType) {
                                case Engine::Graphics::Lighting::Type::DIRECTIONAL:
                                    ImGui::Text("type: directional");
                                    break;
                                case Engine::Graphics::Lighting::Type::POINT:
                                    ImGui::Text("type: point");
                                    break;
                                case Engine::Graphics::Lighting::Type::SPOT:
                                    ImGui::Text("type: spot");
                                    break;
                                default:
                                    // TODO: Throw exception about unknown lighting type
                                    break;
                            }

                            ImGui::SliderFloat("intensity", &selectedEntity->light->intensity, 0, 10.0);
                            ImGui::ColorEdit3("color", &selectedEntity->light->color[0]);
                            ImGui::SliderFloat3("ambient", &selectedEntity->light->ambient[0], 0.0, 1.0);
                            ImGui::SliderFloat3("diffuse", &selectedEntity->light->diffuse[0], 0.0, 1.0);
                            ImGui::SliderFloat3("specular", &selectedEntity->light->specular[0], 0.0, 1.0);

                            switch (selectedEntity->light->lightType) {
                                case Engine::Graphics::Lighting::Type::DIRECTIONAL:
                                    ImGui::SliderFloat3("direction", &selectedEntity->light->direction[0], -10.0, 10.0);
                                    break;
                                case Engine::Graphics::Lighting::Type::POINT:
                                    ImGui::SliderFloat("constant", &selectedEntity->light->constant, 0, 1.0);
                                    ImGui::SliderFloat("linear", &selectedEntity->light->linear, 0, 1.0);
                                    ImGui::SliderFloat("quadratic", &selectedEntity->light->quadratic, 0, 1.0);
                                    break;
                                case Engine::Graphics::Lighting::Type::SPOT:
                                    // TODO: Pass cutOff and outerCutOff as a cos calculated
                                    ImGui::SliderFloat("cutOff", &selectedEntity->light->cutOff, 0, 90.0);
                                    ImGui::SliderFloat("outerCutOff", &selectedEntity->light->outerCutOff, 0, 90.0);
                                    ImGui::SliderFloat("constant", &selectedEntity->light->constant, 0, 1.0);
                                    ImGui::SliderFloat("linear", &selectedEntity->light->linear, 0, 1.0);
                                    ImGui::SliderFloat("quadratic", &selectedEntity->light->quadratic, 0, 1.0);
                                    ImGui::SliderFloat3("direction", &selectedEntity->light->direction[0], -10.0, 10.0);
                                    break;
                                default:
                                    // TODO: Throw exception about unknown lighting type
                                    break;
                            }
                        }
                    }

                    if (selectedEntity->HasComponent(Ecs::Component::Type::CAMERA)) {
                        if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_None)) {
                            // Select projection type selector
                            static int projection = selectedEntity->camera->projection;
                            const char* projectionNames[] = {"Perspective", "Orthographic"};

                            if (ImGui::SliderInt("projection", &projection, 0, 1, projectionNames[projection])) {
                                selectedEntity->camera->projection = Graphics::Camera::Projection(projection);
                            }

                            // TODO: Add viewport settings - https://docs.unity3d.com/Manual/class-Camera.html

                            if (ImGui::SliderFloat("yaw", &selectedEntity->camera->yaw, -180, 180)) {
                                static_cast<Engine::Graphics::Camera::Camera*>(selectedEntity.get())->updateVectors();
                            }
                            if (ImGui::SliderFloat("pitch", &selectedEntity->camera->pitch, -180, 180)) {
                                static_cast<Engine::Graphics::Camera::Camera*>(selectedEntity.get())->updateVectors();
                            }
                            ImGui::SliderFloat("zoom", &selectedEntity->camera->zoom, 0, 90.0);
                            ImGui::SliderFloat("speed", &selectedEntity->camera->speed, 0, 90.0);
                            ImGui::SliderFloat("sensitivity", &selectedEntity->camera->sensitivity, 0, 90.0);

                            ImGui::SliderFloat3("up", &selectedEntity->camera->up[0], -10.0, 10.0);
                            ImGui::SliderFloat3("front", &selectedEntity->camera->front[0], -10.0, 10.0);
                            ImGui::SliderFloat3("right", &selectedEntity->camera->right[0], -10.0, 10.0);
                        }
                    }
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


        void EngineEditor::DrawSceneGraph(std::shared_ptr<Scene::Node> node) {
            if (node->children.empty() && node->entities.empty()) {
                ImGui::TreeNodeEx(node->name.c_str(), ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_SpanFullWidth);
            }
            else {
                ImGui::TreeNode(node->name.c_str());
            }

            for (std::shared_ptr<Object> entity : node->entities) {
                ImGui::TreeNodeEx(entity->name.c_str(), ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_SpanFullWidth);

                if (ImGui::IsItemClicked()) {
                    SelectEntity(entity);
                }
            }

            if (node->children.empty()) {
                ImGui::TreeNodeEx(node->name.c_str(), ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_SpanFullWidth);
            }
            else {
                if (ImGui::TreeNodeEx(node->name.c_str(), ImGuiTreeNodeFlags_SpanFullWidth)) {
                    for (std::shared_ptr<Scene::Node> node : node->children) {
                        DrawSceneGraph(node);
                    }

                    ImGui::TreePop();
                }
            }
        }


        void EngineEditor::SelectEntity(std::shared_ptr<Object> entity) {
            selectedEntity = entity;

            logger->info(fmt::format("Selected entity: {}", entity->name));

            if (entity->HasComponent(Ecs::Component::Type::TRANSFORM)) {
                logger->info("Entity support transform");
            }

            if (entity->HasComponent(Ecs::Component::Type::MATERIAL)) {
                logger->info("Entity support material");
            }

            if (entity->HasComponent(Ecs::Component::Type::MESH)) {
                logger->info("Entity support mesh");
            }

            if (entity->HasComponent(Ecs::Component::Type::LIGHT)) {
                logger->info("Entity support light");
            }
        }
    }
}
