#include <optional>
#include <functional>
#include <memory>
#include <vector>
#include <iostream>
#include <fmt/format.h>
#include <string>
#include <fstream>
#include <streambuf>
#include <filesystem>
#include <iostream>

#define GLEW_STATIC
#define GLM_FORCE_RADIANS
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SOIL/SOIL.h>

#include "engine.hpp"
#include "app.hpp"
#include "meta.hpp"
#include "editor.hpp"
#include "tools/model.hpp"
#include "tools/logger.hpp"
#include "tools/debug/axes/base.hpp"
#include "tools/debug/grid/base.hpp"
#include "core/render/buffer/base.hpp"
#include "core/render/shader/base.hpp"
#include "core/render/texture/base.hpp"
#include "core/render/texture/cubemap/base.hpp"
#include "core/scene/scene.hpp"
#include "core/object/object.hpp"
#include "core/window/base.hpp"
#include "core/window/glfw.hpp"
#include "core/event/observer.hpp"
#include "core/geometry/primitives.hpp"
#include "core/graphics/lighting/base.hpp"
#include "core/graphics/cameras/camera.hpp"


bool keys[1024];
bool mouseCamera = false;
bool mouseScrollback = false;

float lastX = 0;
float lastY = 0;
float lastOffsetX = 0.0f;
float lastOffsetY = 0.0f;
float mouseScrollbackY = 0.0f;
float deltaTime = 0.0f;
float lastFrame = 0.0f;

GLuint screenWidth = 1920;
GLuint screenHeight = 1080;


class CameraController {
public:
    Engine::Graphics::Camera::Camera *camera = nullptr;

    CameraController(Engine::Graphics::Camera::Camera *targetCamera) {
        camera = targetCamera;
    }

    void update(float delta) {
        float velocity = camera->speed * delta;

        if(keys[GLFW_KEY_W]) {
            camera->position += camera->front * velocity;
        }
        else if(keys[GLFW_KEY_S]) {
            camera->position -= camera->front * velocity;
        }
        else if(keys[GLFW_KEY_A]) {
            camera->position -= camera->right * velocity;
        }
        else if(keys[GLFW_KEY_D]) {
            camera->position += camera->right * velocity;
        }
    }

    void processMouse(float x, float y) {
        x *= camera->sensitivity;
        y *= camera->sensitivity;

        camera->yaw += x;
        camera->pitch += y;

        if (camera->pitch > 89.0f) {
            camera->pitch = 89.0f;
        }

        if (camera->pitch < -89.0f) {
            camera->pitch = -89.0f;
        }

        camera->updateVectors();
    }

    void processMouseScroll(float y) {
        if (1.0f <= camera->zoom && camera->zoom <= 45.0f) {
            camera->zoom -= y;
        }

        if (camera->zoom < 1.0f) {
            camera->zoom = 1.0f;
        }

        if (camera->zoom > 45.0f) {
            camera->zoom = 45.0f;
        }
    }
};


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mode);

    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    if (key >= 0 && key < 1024) {
        if(action == GLFW_PRESS)
            keys[key] = true;
        else if(action == GLFW_RELEASE)
            keys[key] = false;	
    }
}


void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);

    ImGuiIO& io = ImGui::GetIO();

    // For keyboard io.WantCaptureKeyboard
    if (io.WantCaptureMouse == false) {
        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
            lastOffsetX = 0;
            lastOffsetY = 0;

            mouseCamera = true;
        }
        else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
            mouseCamera = false;
        }
    }
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    ImGui_ImplGlfw_CursorPosCallback(window, xpos, ypos);

    ImGuiIO& io = ImGui::GetIO();

    if (io.WantCaptureMouse == false) {
        lastOffsetX = xpos - lastX;
        lastOffsetY = lastY - ypos;
    
        lastX = xpos;
        lastY = ypos;
    }
}	


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);

    ImGuiIO& io = ImGui::GetIO();

    if (io.WantCaptureMouse == false) {
        mouseScrollback = true;
        mouseScrollbackY = yoffset;
    }
}


void window_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}


class UserApplication : public Engine::EngineApplication {
public:
    // Engine::Render::Cubemap *cubemap;
    // Engine::Render::ShaderProgram *skyboxShader = nullptr;
    // std::unique_ptr<Engine::Render::VertexArray> skyboxVAO;
    // std::unique_ptr<Engine::Render::VertexBuffer> skyboxVBO;

    Tool::Debug::DebugAxes *debugAxes = nullptr;
    Tool::Debug::DebugFloorGrid *debugFloorGrid = nullptr;

    std::unique_ptr<Engine::Graphics::Camera::Camera> camera;
    std::unique_ptr<CameraController> cameraController;

    void Startup() {
        logger->trace(std::string("Startup"));

        this->window->settings = {
            screenWidth, screenHeight, false, true, "Application"
        };
    }

    void Run() {
        glfwSetWindowSizeCallback(static_cast<Engine::Window::GLFWWindowProvider*>(this->window)->object, window_size_callback);

        // TODO: Need custom event system because events need for application and editor
        // bd->PrevUserCallbackWindowFocus = glfwSetWindowFocusCallback(window, ImGui_ImplGlfw_WindowFocusCallback);
        // bd->PrevUserCallbackCursorEnter = glfwSetCursorEnterCallback(window, ImGui_ImplGlfw_CursorEnterCallback);
        // bd->PrevUserCallbackCursorPos = glfwSetCursorPosCallback(window, ImGui_ImplGlfw_CursorPosCallback);
        // bd->PrevUserCallbackMousebutton = glfwSetMouseButtonCallback(window, ImGui_ImplGlfw_MouseButtonCallback);
        // bd->PrevUserCallbackScroll = glfwSetScrollCallback(window, ImGui_ImplGlfw_ScrollCallback);
        // bd->PrevUserCallbackKey = glfwSetKeyCallback(window, ImGui_ImplGlfw_KeyCallback);
        // bd->PrevUserCallbackChar = glfwSetCharCallback(window, ImGui_ImplGlfw_CharCallback);
        // bd->PrevUserCallbackMonitor = glfwSetMonitorCallback(ImGui_ImplGlfw_MonitorCallback);

        // Camera and controller setup
        camera = std::make_unique<Engine::Graphics::Camera::Camera>();
        cameraController = std::make_unique<CameraController>(camera.get());

        // std::filesystem::path cwd = std::filesystem::current_path();

        // this->skyboxShader = Engine::Render::ShaderProgram::GetInstance();
        // this->skyboxShader->Build(
        //     cwd / "resources" / "shaders" / "skybox" / "shader.vert",
        //     cwd / "resources" / "shaders" / "skybox" / "shader.frag"
        // );
        
        // float skyboxVertices[] = {
        //     // positions          
        //     -1000.0f,  1000.0f, -1000.0f,
        //     -1000.0f, -1000.0f, -1000.0f,
        //      1000.0f, -1000.0f, -1000.0f,
        //      1000.0f, -1000.0f, -1000.0f,
        //      1000.0f,  1000.0f, -1000.0f,
        //     -1000.0f,  1000.0f, -1000.0f,

        //     -1000.0f, -1000.0f,  1000.0f,
        //     -1000.0f, -1000.0f, -1000.0f,
        //     -1000.0f,  1000.0f, -1000.0f,
        //     -1000.0f,  1000.0f, -1000.0f,
        //     -1000.0f,  1000.0f,  1000.0f,
        //     -1000.0f, -1000.0f,  1000.0f,

        //      1000.0f, -1000.0f, -1000.0f,
        //      1000.0f, -1000.0f,  1000.0f,
        //      1000.0f,  1000.0f,  1000.0f,
        //      1000.0f,  1000.0f,  1000.0f,
        //      1000.0f,  1000.0f, -1000.0f,
        //      1000.0f, -1000.0f, -1000.0f,

        //     -1000.0f, -1000.0f,  1000.0f,
        //     -1000.0f,  1000.0f,  1000.0f,
        //      1000.0f,  1000.0f,  1000.0f,
        //      1000.0f,  1000.0f,  1000.0f,
        //      1000.0f, -1000.0f,  1000.0f,
        //     -1000.0f, -1000.0f,  1000.0f,

        //     -1000.0f,  1000.0f, -1000.0f,
        //      1000.0f,  1000.0f, -1000.0f,
        //      1000.0f,  1000.0f,  1000.0f,
        //      1000.0f,  1000.0f,  1000.0f,
        //     -1000.0f,  1000.0f,  1000.0f,
        //     -1000.0f,  1000.0f, -1000.0f,

        //     -1000.0f, -1000.0f, -1000.0f,
        //     -1000.0f, -1000.0f,  1000.0f,
        //      1000.0f, -1000.0f, -1000.0f,
        //      1000.0f, -1000.0f, -1000.0f,
        //     -1000.0f, -1000.0f,  1000.0f,
        //      1000.0f, -1000.0f,  1000.0f
        // };
        // std::vector<std::filesystem::path> faces = {
        //     cwd / "resources" / "textures" / "skybox" / "default" / "right.jpg",
        //     cwd / "resources" / "textures" / "skybox" / "default" / "left.jpg",
        //     cwd / "resources" / "textures" / "skybox" / "default" / "top.jpg",
        //     cwd / "resources" / "textures" / "skybox" / "default" / "bottom.jpg",
        //     cwd / "resources" / "textures" / "skybox" / "default" / "front.jpg",
        //     cwd / "resources" / "textures" / "skybox" / "default" / "back.jpg"
        // };
        // cubemap = Engine::Render::Cubemap::GetInstance();
        // cubemap->Build(faces);

        // // Bind skybox
        // this->skyboxVAO = Engine::Render::VertexArray::GetInstance();
        // this->skyboxVBO = Engine::Render::VertexBuffer::GetInstance();

        // this->skyboxVAO->bind();
        // this->skyboxVBO->bind(skyboxVertices, sizeof(skyboxVertices));
    
        // this->skyboxVAO->layout(3, 3 * sizeof(float), 0);

        // this->skyboxVBO->unbind();
        // this->skyboxVAO->unbind();
        // // End skybox

        // Add geometry primitives to the scene
        std::shared_ptr<Engine::Geometry::Plane> plane = std::make_shared<Engine::Geometry::Plane>();
        std::shared_ptr<Engine::Geometry::Cube> cube = std::make_shared<Engine::Geometry::Cube>();
        std::shared_ptr<Engine::Geometry::Cube> cube2 = std::make_shared<Engine::Geometry::Cube>();

        cube->transform->position = {-3.0f, 1.5f, 2.0f};
        cube2->transform->position = {2.5f, -0.5f, -3.0f};

        scene->root->entities.push_back(plane);
        scene->root->entities.push_back(cube);
        scene->root->entities.push_back(cube2);
        // End geometry primitives

        // Add light to the scene
        std::shared_ptr<Engine::Graphics::Lighting::Light> light = 
            std::make_shared<Engine::Graphics::Lighting::Light>();
        
        light->transform->position = {-3.0f, 3.0f, -3.0f};

        scene->lighting.push_back(light);
        // End light

        GLFWwindow *window = static_cast<Engine::Window::GLFWWindowProvider*>(this->window)->object;
        glfwSetMouseButtonCallback(window, mouse_button_callback);
        // glfwSetCursorPosCallback(window, cursor_position_callback);
        glfwSetKeyCallback(window, key_callback);
        glfwSetCursorPosCallback(window, mouse_callback);
        glfwSetScrollCallback(window, scroll_callback);

        this->debugAxes = Tool::Debug::DebugAxes::GetInstance();
        this->debugFloorGrid = Tool::Debug::DebugFloorGrid::GetInstance();
    }

    void Update() {
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        cameraController->update(deltaTime);

        if (mouseCamera) {
            cameraController->processMouse(lastOffsetX, lastOffsetY);

            lastOffsetX = 0.0f;
            lastOffsetY = 0.0f;
        }

        if (mouseScrollback) {
            cameraController->processMouseScroll(mouseScrollbackY);

            mouseScrollback = false;
            mouseScrollbackY = 0.0f;
        }

        glm::mat4 model(1.0f);
        glm::mat4 view(1.0f);
        glm::mat4 projection(1.0f);

        view = camera->getViewMatrix();
        projection = camera->getProjectionMatrix();
 
        render->RenderScene(scene, projection * view * model, camera->position);

        // // Draw skybox
        // glDepthFunc(GL_LEQUAL);
        // skyboxShader->Use();
        // this->skyboxShader->UniformMatrix("view", view);
        // this->skyboxShader->UniformMatrix("projection", projection);
        // glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap->object);

        // this->skyboxVAO->bind();

        // glDrawArrays(GL_TRIANGLES, 0, 36);

        // this->skyboxVAO->unbind();

        // glDepthFunc(GL_LESS);
        // // End skybox

        // TODO: Move to the editor as debug flag
        // this->debugAxes->SetMVP(projection * view);
        // this->debugAxes->Enable();

        // this->debugFloorGrid->SetMVP(projection * view);
        // this->debugFloorGrid->Enable();
    }

    void Shutdown() {
        logger->trace(std::string("Shutdown"));
    }
};


int main() {
    UserApplication *application = new UserApplication();
    Engine::Engine engine(application);

    engine.Startup();

    try {
        engine.Run();
    } catch (const std::exception& error) {
        std::cerr << error.what() << std::endl;

        delete application;

        return EXIT_FAILURE;
    }

    delete application;

    return EXIT_SUCCESS;
}
