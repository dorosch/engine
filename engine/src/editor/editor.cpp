#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "engine.hpp"
#include "editor/editor.hpp"


namespace Engine {
    namespace Editor {
        EngineEditor::EngineEditor(EngineApplication *app) {
            logger->trace("constructor");

            this->app = app;
        }


        void EngineEditor::Init() {
            logger->trace("Init");

            // TODO: Move this functionality to the window provider
            const char* glsl_version = "#version 130";
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

            IMGUI_CHECKVERSION();
            ImGui::CreateContext();
            ImGuiIO& io = ImGui::GetIO(); (void)io;

            ImGui::StyleColorsDark();

            // Setup Platform/Renderer backends
            ImGui_ImplGlfw_InitForOpenGL(static_cast<Window::GLFWWindowProvider*>(this->app->windowManager->window)->object, true);
            ImGui_ImplOpenGL3_Init(glsl_version);
        }


        void EngineEditor::Update() {
            logger->trace("Update");

            bool show_demo_window = true;
            bool show_another_window = false;
            ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

            // Start the Dear ImGui frame
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            // Show the big demo window
            if (show_demo_window) {
                ImGui::ShowDemoWindow(&show_demo_window);
            }

            // Show a simple window
            {
                static float f = 0.0f;
                static int counter = 0;

                ImGui::Begin("Hello, world!");

                ImGui::Text("This is some useful text.");
                ImGui::Checkbox("Demo Window", &show_demo_window);
                ImGui::Checkbox("Another Window", &show_another_window);

                ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
                ImGui::ColorEdit3("clear color", (float*)&clear_color);

                if (ImGui::Button("Button")) {
                    counter++;
                }

                ImGui::SameLine();
                ImGui::Text("counter = %d", counter);

                ImGui::Text(
                    "Application average %.3f ms/frame (%.1f FPS)",
                    1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate
                );
                ImGui::End();
            }

            if (show_another_window) {
                ImGui::Begin("Another Window", &show_another_window);
                ImGui::Text("Hello from another window!");

                if (ImGui::Button("Close Me")) {
                    show_another_window = false;
                }

                ImGui::End();
            }

            ImGui::Render();

            // TODO: Move clear window to the window provider
            int display_w, display_h;
            glfwGetFramebufferSize(static_cast<Window::GLFWWindowProvider*>(this->app->windowManager->window)->object, &display_w, &display_h);
            glViewport(0, 0, display_w, display_h);
            glClearColor(0, 0, 0, 0);
            glClear(GL_COLOR_BUFFER_BIT);

            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        }


        void EngineEditor::Shutdown() {
            logger->trace("Shutdown");
        }
    }
}
