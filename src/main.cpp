#define GLEW_STATIC
#define GLM_FORCE_RADIANS

#include <memory>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "engine.hpp"
#include "tools/debug/axes/base.hpp"
#include "tools/debug/grid/base.hpp"
#include "core/window/glfw.hpp"
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
    Tool::Debug::DebugAxes *debugAxes = nullptr;
    Tool::Debug::DebugFloorGrid *debugFloorGrid = nullptr;

    std::unique_ptr<Engine::Graphics::Camera::Camera> camera;
    std::unique_ptr<CameraController> cameraController;

    void Startup() {
        this->window->settings = {
            screenWidth, screenHeight, false, true, "Application"
        };
    }

    void Run() {
        GLFWwindow *window = static_cast<Engine::Window::GLFWWindowProvider*>(this->window)->object;
        glfwSetMouseButtonCallback(window, mouse_button_callback);
        glfwSetKeyCallback(window, key_callback);
        glfwSetCursorPosCallback(window, mouse_callback);
        glfwSetScrollCallback(window, scroll_callback);
        glfwSetWindowSizeCallback(window, window_size_callback);

        // Camera and controller setup
        camera = std::make_unique<Engine::Graphics::Camera::Camera>();
        cameraController = std::make_unique<CameraController>(camera.get());

        // Add geometry primitives to the scene
        std::shared_ptr<Engine::Geometry::Plane> plane = std::make_shared<Engine::Geometry::Plane>();
        std::shared_ptr<Engine::Geometry::Cube> cube = std::make_shared<Engine::Geometry::Cube>();
        std::shared_ptr<Engine::Geometry::Cube> cube2 = std::make_shared<Engine::Geometry::Cube>();

        plane->transform->position = {0.0f, 0.1f, 0.0f};
        plane->transform->rotation = {0.0f, 1.0f, 1.0f};
        plane->transform->scale = {20.0f, 20.0f, 1.0f};

        cube->transform->position = {-3.0f, 1.5f, 2.0f};
        cube->transform->rotation = {0.0f, 1.0f, 1.0f};
        cube2->transform->position = {2.5f, 1.5f, -3.0f};
        cube2->transform->rotation = {0.0f, 1.0f, 1.0f};

        cube->material = std::make_unique<Engine::Ecs::Component::Material>();
        cube->material->ambient = glm::vec3(1.0f, 0.5f, 0.31f);
        cube->material->diffuse = glm::vec3(1.0f, 0.5f, 0.31f);
        cube->material->specular = glm::vec3(0.5f, 0.5f, 0.5f);
        cube->material->shininess = 32.0f;

        scene->root->entities.push_back(plane);
        scene->root->entities.push_back(cube);
        scene->root->entities.push_back(cube2);
        // End geometry primitives

        // Add sun to the scene
        std::shared_ptr<Engine::Graphics::Lighting::SpotLight> sun = 
            std::make_shared<Engine::Graphics::Lighting::SpotLight>();
        std::shared_ptr<Engine::Graphics::Lighting::PointLight> point = 
            std::make_shared<Engine::Graphics::Lighting::PointLight>();
        std::shared_ptr<Engine::Graphics::Lighting::PointLight> point2 = 
            std::make_shared<Engine::Graphics::Lighting::PointLight>();

        sun->name = "Sun";
        sun->light->intensity = 2.0f;
        sun->light->constant = 1.0f;
        sun->light->linear = 0.045;
        sun->light->quadratic = 0.0075;
        sun->light->direction = {3.0f, -3.0f, 1.0f};
        sun->transform->position = {-3.0f, 3.0f, -3.0f};
        sun->transform->scale = {0.1f, 0.1f, 0.1f};

        point->transform->position = {3.0f, 3.0f, 3.0f};
        point->transform->scale = {0.1f, 0.1f, 0.1f};
        point2->transform->position = {4.0f, 2.0f, 2.0f};
        point2->transform->scale = {0.1f, 0.1f, 0.1f};

        scene->lighting.push_back(sun);
        scene->lighting.push_back(point);
        scene->lighting.push_back(point2);
        // End sun light

        this->debugAxes = Tool::Debug::DebugAxes::GetInstance();
        this->debugFloorGrid = Tool::Debug::DebugFloorGrid::GetInstance();
    }

    void Update(float delta) {
        glm::mat4 view = camera->getViewMatrix();
        glm::mat4 projection = camera->getProjectionMatrix();

        cameraController->update(delta);

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
 
        // TODO: Move to the editor as debug flag
        this->debugAxes->SetMVP(projection * view);
        this->debugAxes->Enable();

        this->debugFloorGrid->SetMVP(projection * view);
        this->debugFloorGrid->Enable();

        render->RenderScene(scene, projection, view, camera->position);
    }

    void Shutdown() {
        logger->trace(std::string("Shutdown"));
    }
};


int main() {
    UserApplication *application = new UserApplication();
    Engine::Engine engine(application);

    try {
        engine.Startup();

        engine.Run();
    } catch (const std::exception& error) {
        std::cerr << error.what() << std::endl;

        delete application;

        return EXIT_FAILURE;
    }

    delete application;

    return EXIT_SUCCESS;
}
