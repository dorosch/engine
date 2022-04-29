#include "meta.hpp"
#include "editor.hpp"
#include <iostream>


float position[] = {0.0, 0.0, 0.0};
float color[4] = {1.0f, 1.0f, 1.0f, 1.0f};


namespace Engine {
    namespace Editor {
        void Editor::Startup(GLFWwindow *window) {
            this->window = window;

            logger->trace(std::string("Init"));

            IMGUI_CHECKVERSION();
            ImGui::CreateContext();
            ImGuiIO& io = ImGui::GetIO(); (void)io;

            ImGui::StyleColorsDark();

            // Setup Platform/Renderer backends
            ImGui_ImplGlfw_InitForOpenGL(this->window, true);
            ImGui_ImplOpenGL3_Init(GLSL_VERSION);
        }


        void Editor::Update() {
            static bool closed = false;
            static bool wireframe = false;

            logger->trace(std::string("Update"));

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

            if (wireframe) {
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            }
            else {
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            }

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            int display_w, display_h;
            glfwGetFramebufferSize(this->window, &display_w, &display_h);
            glViewport(0, 0, display_w, display_h);
        }


        void Editor::Shutdown() {
            logger->trace(std::string("Shutdown"));

            ImGui_ImplOpenGL3_Shutdown();
            ImGui_ImplGlfw_Shutdown();
            ImGui::DestroyContext();
        }
    }
}
