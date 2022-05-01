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
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "meta.hpp"
#include "editor.hpp"

#include "tools/model.hpp"
#include "tools/logger.hpp"
#include "tools/debug/axes/base.hpp"
#include "tools/debug/grid/base.hpp"
#include "core/render/api.hpp"
#include "core/render/buffer/base.hpp"
#include "core/render/shader/base.hpp"
#include "core/render/texture/base.hpp"


namespace Engine {
    using namespace Tool::Logger;


    namespace Scene {
        enum CameraDirection {
            FORWARD,
            BACKWARD,
            LEFT,
            RIGHT
        };


        class Camera {

        };


        class OpenglCamera : public Camera {
        public:
            glm::vec3 Position;
            glm::vec3 Front;
            glm::vec3 Up;
            glm::vec3 Right;
            glm::vec3 WorldUp;
    
            // Eular Angles
            GLfloat Yaw;
            GLfloat Pitch;
    
            // Camera options
            GLfloat MovementSpeed;
            GLfloat MouseSensitivity;
            GLfloat Zoom;

            OpenglCamera() {
                this->Position = glm::vec3(5.0f, 5.0f, 5.0f);
                this->WorldUp = glm::vec3(0.0f, 5.0f, 0.0f);
                this->Yaw = -90.0f;
                this->Pitch = 0.0f;
                this->MovementSpeed = 10.0f;
                this->MouseSensitivity = 0.25f;
                this->Zoom = 60.0f;

                this->updateCameraVectors();
            }

            virtual ~OpenglCamera() {}

            glm::mat4 GetViewMatrix() {
                return glm::lookAt(this->Position, this->Position + this->Front, this->Up);
            }

            void ProcessKeyboard(CameraDirection direction, GLfloat deltaTime) {
                GLfloat velocity = this->MovementSpeed * deltaTime;
        
                if (direction == FORWARD)
                    this->Position += this->Front * velocity;
                if (direction == BACKWARD)
                    this->Position -= this->Front * velocity;
                if (direction == LEFT)
                    this->Position -= this->Right * velocity;
                if (direction == RIGHT)
                    this->Position += this->Right * velocity;
            }

            void ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch = true) {
                xoffset *= this->MouseSensitivity;
                yoffset *= this->MouseSensitivity;

                this->Yaw   += xoffset;
                this->Pitch += yoffset;

                // Make sure that when pitch is out of bounds, screen doesn't get flipped
                if (constrainPitch) {
                    if (this->Pitch > 89.0f)
                        this->Pitch = 89.0f;
                    if (this->Pitch < -89.0f)
                        this->Pitch = -89.0f;
                }

                // Update Front, Right and Up Vectors using the updated Eular angles
                this->updateCameraVectors();
            }

            // Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
            void ProcessMouseScroll(GLfloat yoffset) {
                if (this->Zoom >= 1.0f && this->Zoom <= 45.0f)
                    this->Zoom -= yoffset;
                if (this->Zoom <= 1.0f)
                    this->Zoom = 1.0f;
                if (this->Zoom >= 45.0f)
                    this->Zoom = 45.0f;
            }

            void updateCameraVectors() {
                // Calculate the new Front vector
                glm::vec3 front;
                front.x = cos(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
                front.y = sin(glm::radians(this->Pitch));
                front.z = sin(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
                this->Front = glm::normalize(front);

                // Also re-calculate the Right and Up vector
                this->Right = glm::normalize(glm::cross(this->Front, this->WorldUp));
                this->Up = glm::normalize(glm::cross(this->Right, this->Front));
            }
        };
    }


    namespace Window {
        enum Provider {
            GLFW,
        };


        struct Settings {
            int width;
            int height;
            bool fullScreen;
            bool canResize;
            const char *title;
        };


        class WindowProvider {
        public:
            Provider provider;
            Settings settings = {
                800, 600, false, false, "Application"
            };

            virtual ~WindowProvider() {};
            virtual void Startup() = 0;
            virtual void Create() = 0;
            virtual void Update() = 0;
            virtual void Shutdown() = 0;
            virtual bool IsOpen() = 0;
        };


        class GLFWWindowProvider : public WindowProvider {
        private:
            GLFWmonitor *primaryMonitor = nullptr;
            std::unique_ptr<Logger> logger = std::make_unique<Logger>("glfw");

        public:
            GLFWwindow *object = nullptr;
            Provider provider = Provider::GLFW;

            void Startup() {
                logger->trace(std::string("Startup"));

                glfwInit();

                glfwWindowHint(GLFW_SAMPLES, 4);

                glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
                glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
            }

            void Create() {
                logger->trace(std::string("Create"));

                if (!this->settings.canResize) {
                    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
                }

                if (this->settings.fullScreen) {
                    this->primaryMonitor = glfwGetPrimaryMonitor();
                }

                this->object = glfwCreateWindow(
                    this->settings.width,
                    this->settings.height,
                    this->settings.title,
                    this->primaryMonitor,
                    nullptr
                );

                if (!this->object) {
                    throw std::runtime_error("Window creation error");
                }

                glfwMakeContextCurrent(this->object);
                glfwSetFramebufferSizeCallback(this->object, framebufferChangeSizeCallback);
            }

            void Update() {
                glfwSwapBuffers(this->object);
            }

            void Shutdown() {
                logger->trace(std::string("Shutdown"));

                glfwDestroyWindow(this->object);
            }

            bool IsOpen() {
                return !glfwWindowShouldClose(this->object);
            }

            static void framebufferChangeSizeCallback(GLFWwindow* window, int width, int height) {
                glViewport(0, 0, width, height);
            }
        };
    }


    class Application {
        /**
         * Custom engine application class.
         * 
         * When creating an application using the engine, the entry 
         * point is a class inherited from this one. Also, the custom 
         * application needs to override three methods:
         *     Startup()
         *     Update()
         *     Shutdown()
         * 
         * A user application should not override a constructor or 
         * destructor without calling the parent construct or destructor. 
         */

    public:
        std::unique_ptr<Logger> logger = std::make_unique<Logger>("app");

        Window::Provider provider = Window::Provider::GLFW;
        Window::WindowProvider *window = nullptr;
        Editor::Editor *editor = nullptr;

        Application() {
            logger->trace(std::string("constructor"));

            // Initialize window provider
            if (!this->provider) {
                this->provider = Window::Provider::GLFW;
            }

            switch (this->provider) {
                case Window::Provider::GLFW:
                    this->window = new Window::GLFWWindowProvider();
                    break;
                default:
                    throw std::logic_error("Undefined WindowProvider");
            }

            this->editor = new Editor::Editor();
        }

        virtual ~Application() {
            logger->trace(std::string("destructor"));

            delete this->editor;
            delete this->window;
        }

        virtual void Startup() = 0;
        virtual void Run() = 0;
        virtual void Update() = 0;
        virtual void Shutdown() = 0;
    };


    class Engine {
        /**
         * Main engine class.
         * 
         * When used, the engine must have a custom user application. 
         * Further work of the engine comes down to starting 
         * main loop and updating managers and the user application.
         */

    private:
        Application *app;

    public:
        std::unique_ptr<Logger> logger = std::make_unique<Logger>("engine");

        Engine(Application *application) {
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
            this->app->window->Startup();
            this->app->Startup();

            logger->info(fmt::format("engine version: {}", ENGINE_VERSION));
            logger->info(fmt::format("glsl version: {}", GLSL_VERSION));

            switch (Render::BackendAPI) {
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

            this->app->window->Create();
            this->app->editor->Startup(
                static_cast<Window::GLFWWindowProvider*>(this->app->window)->object
            );

            // TODO: Move all gl* functions to the render backend
            glewExperimental = GL_TRUE;
            glewInit();

            glEnable(GL_DEPTH_TEST);

            this->app->Run();

            while (this->app->window->IsOpen()) {
                glfwPollEvents();

                glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                this->app->Update();
                this->app->editor->Update();
                this->app->window->Update();
            }

            this->app->editor->Shutdown();
            this->app->window->Shutdown();
            this->app->Shutdown();
        }
    };
}


// ***************************************************************************************
// ************************************* APPLICATION *************************************
// ***************************************************************************************


extern float position[];
extern float color[];


Engine::Scene::OpenglCamera camera;
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;
bool mouseCamera = false;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

GLuint screenWidth = 1920, screenHeight = 1080;


// Moves/alters the camera positions based on user input
void Do_Movement() {
    // Camera controls
    if(keys[GLFW_KEY_W])
        camera.ProcessKeyboard(Engine::Scene::CameraDirection::FORWARD, deltaTime);
    if(keys[GLFW_KEY_S])
        camera.ProcessKeyboard(Engine::Scene::CameraDirection::BACKWARD, deltaTime);
    if(keys[GLFW_KEY_A])
        camera.ProcessKeyboard(Engine::Scene::CameraDirection::LEFT, deltaTime);
    if(keys[GLFW_KEY_D])
        camera.ProcessKeyboard(Engine::Scene::CameraDirection::RIGHT, deltaTime);
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mode);

    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if (key >= 0 && key < 1024)
    {
        if(action == GLFW_PRESS)
            keys[key] = true;
        else if(action == GLFW_RELEASE)
            keys[key] = false;	
    }
}

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos){

}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);

    ImGuiIO& io = ImGui::GetIO();
    if (!io.WantCaptureMouse) {

        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
            mouseCamera = true;
        }
        else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
            mouseCamera = false;
        }
    }
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    ImGui_ImplGlfw_CursorPosCallback(window, xpos, ypos);

    if(firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    GLfloat xoffset = xpos - lastX;
    GLfloat yoffset = lastY - ypos;
    
    lastX = xpos;
    lastY = ypos;

    if (mouseCamera) {
        camera.ProcessMouseMovement(xoffset, yoffset);
    }
}	


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);

    camera.ProcessMouseScroll(yoffset);
}


class UserApplication : public Engine::Application {
public:
    GLuint VAO, modelVAO;
    Engine::Render::VertexBuffer *VBO = nullptr;
    Engine::Render::VertexBuffer *modelVBO = nullptr;
    // Engine::Render::OpenglIndexBuffer *EBO = nullptr;
    Engine::Render::ShaderProgram *shader = nullptr;
    Engine::Render::ShaderProgram *shaderModel = nullptr;
    Engine::Render::Texture *texture = nullptr;

    Tool::ObjModel *model = nullptr;
    Tool::Debug::DebugAxes *debugAxes = nullptr;
    Tool::Debug::DebugFloorGrid *debugFloorGrid = nullptr;

    void Startup() {
        logger->trace(std::string("Startup"));

        this->window->settings = {
            screenWidth, screenHeight, false, true, "Application"
        };
    }

    void Run() {
        // TODO: Need custom event system because events need for application and editor
        // bd->PrevUserCallbackWindowFocus = glfwSetWindowFocusCallback(window, ImGui_ImplGlfw_WindowFocusCallback);
        // bd->PrevUserCallbackCursorEnter = glfwSetCursorEnterCallback(window, ImGui_ImplGlfw_CursorEnterCallback);
        // bd->PrevUserCallbackCursorPos = glfwSetCursorPosCallback(window, ImGui_ImplGlfw_CursorPosCallback);
        // bd->PrevUserCallbackMousebutton = glfwSetMouseButtonCallback(window, ImGui_ImplGlfw_MouseButtonCallback);
        // bd->PrevUserCallbackScroll = glfwSetScrollCallback(window, ImGui_ImplGlfw_ScrollCallback);
        // bd->PrevUserCallbackKey = glfwSetKeyCallback(window, ImGui_ImplGlfw_KeyCallback);
        // bd->PrevUserCallbackChar = glfwSetCharCallback(window, ImGui_ImplGlfw_CharCallback);
        // bd->PrevUserCallbackMonitor = glfwSetMonitorCallback(ImGui_ImplGlfw_MonitorCallback);

        glfwSetMouseButtonCallback(static_cast<Engine::Window::GLFWWindowProvider*>(this->window)->object, mouse_button_callback);
        glfwSetCursorPosCallback(static_cast<Engine::Window::GLFWWindowProvider*>(this->window)->object, cursor_position_callback);
        glfwSetKeyCallback(static_cast<Engine::Window::GLFWWindowProvider*>(this->window)->object, key_callback);
        glfwSetCursorPosCallback(static_cast<Engine::Window::GLFWWindowProvider*>(this->window)->object, mouse_callback);
        glfwSetScrollCallback(static_cast<Engine::Window::GLFWWindowProvider*>(this->window)->object, scroll_callback);

        std::filesystem::path cwd = std::filesystem::current_path();

        this->model = new Tool::ObjModel(cwd / "resources" / "models" / "IS7.obj");
        model->Load();
        // TODO: Fix model deletion
        // delete model;

        this->shaderModel = Engine::Render::ShaderProgram::GetInstance();
        this->shaderModel->Build(
            cwd / "resources" / "shaders" / "model.vert",
            cwd / "resources" / "shaders" / "model.frag"
        );

        this->shader = Engine::Render::ShaderProgram::GetInstance();
        this->shader->Build(
            cwd / "resources" / "shaders" / "position-texture.vert",
            cwd / "resources" / "shaders" / "position-texture.frag"
        );
        this->debugAxes = Tool::Debug::DebugAxes::GetInstance();
        this->debugFloorGrid = Tool::Debug::DebugFloorGrid::GetInstance();

        this->texture = Engine::Render::Texture::GetInstance();
        this->texture->Build(
            cwd / "resources" / "textures" / "container.jpg"
        );

        float vertices[] = {
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
             0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
             0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
             0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
             0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
             0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
        };

        // uint32_t indices[] = {
        //     0, 1, 3,
        //     1, 2, 3
        // };

        // Draw model
        glGenVertexArrays(1, &this->modelVAO);
        this->modelVBO = Engine::Render::VertexBuffer::GetInstance();

        glBindVertexArray(modelVAO);
        glBindBuffer(GL_ARRAY_BUFFER, this->modelVBO->object);
        glBufferData(GL_ARRAY_BUFFER, model->vertices.size() * sizeof(glm::vec4), model->vertices.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(0);

        this->modelVBO->unbind();

        glBindVertexArray(0);
        // End draw model

        glGenVertexArrays(1, &this->VAO);
        this->VBO = Engine::Render::VertexBuffer::GetInstance();
        // this->EBO = new Engine::Render::OpenglIndexBuffer();

        glBindVertexArray(VAO);
        this->VBO->bind(vertices, sizeof(vertices));
        // this->EBO->bind(indices, sizeof(indices) / sizeof(uint32_t));
        
        // Position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
    
        // Color attribute
        // glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
        // glEnableVertexAttribArray(1);
    
        // TexCoord attribute
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);

        this->VBO->unbind();

        glBindVertexArray(0);
    }

    void Update() {
        // glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
        // glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
        // glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);
        // glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f); 
        // glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
        // glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);
        // GLfloat radius = 10.0f;
        // GLfloat camX = sin(glfwGetTime()) * radius;
        // GLfloat camZ = cos(glfwGetTime()) * radius;

        // glm::mat4 view(1.0f);
        // view = glm::lookAt(glm::vec3(camX, 0.0, camZ), glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, 1.0, 0.0));

        // glm::mat4 projection(1.0f);
        // projection = glm::perspective(45.0f, (GLfloat)800 / (GLfloat)600, 0.1f, 100.0f);

        // glm::mat4 rotation(1.0f);
        // rotation = glm::rotate(rotation, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
        // rotation = glm::scale(rotation, glm::vec3(0.5, 0.5, 0.5));

        // rotation = glm::translate(rotation, glm::vec3(0.5f, -0.5f, 0.0f));
        // rotation = glm::rotate(rotation,(GLfloat)glfwGetTime() * 1.0f, glm::vec3(0.0f, 1.0f, 1.0f));

        // std::cout << glm::to_string(rotation) << std::endl;

        Do_Movement();

        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        glm::mat4 view(1.0f);
        view = camera.GetViewMatrix();
        glm::mat4 projection(1.0f);	
        projection = glm::perspective(glm::radians(camera.Zoom), (float)screenWidth/(float)screenHeight, 0.1f, 10000.0f);

        // Draw model
        glm::mat4 model(1.0f);
        this->shaderModel->Use();
        this->shaderModel->UniformMatrix("model", model);
        this->shaderModel->UniformMatrix("view", view);
        this->shaderModel->UniformMatrix("projection", projection);
        
        glBindVertexArray(this->modelVAO);

        glDrawArrays(GL_TRIANGLES, 0, this->model->vertices.size());
        glBindVertexArray(0);
        // End draw model

        this->shader->Use();
        this->shader->UniformColor("testColor", color[0], color[1], color[2]);
        this->shader->UniformPosition("ourPosition", position[0], position[1], position[2]);

        this->shader->UniformMatrix("view", view);
        this->shader->UniformMatrix("projection", projection);

        this->texture->Bind(); 

        glm::vec3 cubePositions[] = {
            glm::vec3( 0.0f,  0.0f,  0.0f), 
            glm::vec3( 2.0f,  5.0f, -15.0f), 
            glm::vec3(-1.5f, -2.2f, -2.5f),  
            glm::vec3(-3.8f, -2.0f, -12.3f),  
            glm::vec3( 2.4f, -0.4f, -3.5f),  
            glm::vec3(-1.7f,  3.0f, -7.5f),  
            glm::vec3( 1.3f, -2.0f, -2.5f),  
            glm::vec3( 1.5f,  2.0f, -2.5f), 
            glm::vec3( 1.5f,  0.2f, -1.5f), 
            glm::vec3(-1.3f,  1.0f, -1.5f)  
        };

        glBindVertexArray(this->VAO);
        for(GLuint i = 0; i < 10; i++) {
            glm::mat4 model(1.0f);
            model = glm::translate(model, cubePositions[i]);

            model = glm::rotate(model, (GLfloat)(1.0f * i), glm::vec3(1.0f, 0.0f, 0.0f));
            this->shader->UniformMatrix("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        glBindVertexArray(0);

        // TODO: Move to the editor as debug flag
        this->debugAxes->SetMVP(projection * view * model);
        this->debugAxes->Enable();

        this->debugFloorGrid->SetMVP(projection * view * model);
        this->debugFloorGrid->Enable();
    }

    void Shutdown() {
        logger->trace(std::string("Shutdown"));

        glDeleteVertexArrays(1, &this->VAO);

        // delete shader;
        // delete shaderModel;
        delete VBO;
        delete this->debugAxes;
        // delete EBO;
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
