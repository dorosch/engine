#include <optional>
#include <functional>
#include <memory>
#include <vector>
#include <iostream>
#include <fmt/format.h>

#define GLEW_STATIC
#define GLM_FORCE_RADIANS
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SOIL/SOIL.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include "meta.hpp"
#include "editor.hpp"


const GLchar* vertexShaderSource = "\n    #version 330 core\n"
    "    layout (location = 0) in vec3 position;\n"
    "    layout (location = 1) in vec2 texCoord;\n"
    "    out vec2 TexCoord;\n"
    "    uniform vec3 ourPosition;\n"
    "    uniform mat4 model;\n"
    "    uniform mat4 view;\n"
    "    uniform mat4 projection;\n"
    "    void main() {\n"
    "        gl_Position =  projection * view * model * vec4(position, 1.0);\n"
    "        TexCoord = texCoord;\n"
    "    }\n\0";
const GLchar* fragmentShaderSource = "\n    #version 330 core\n"
    "    in vec2 TexCoord;\n"
    "    out vec4 color;\n"
    "    uniform sampler2D ourTexture;\n"
    "    void main() {\n"
    "        color = texture(ourTexture, TexCoord);\n"
    "    }\n\0";


// Debug lines
const char *debugVertexShader = "\n"
    "    #version 330 core\n"
    "    layout (location = 0) in vec3 position;\n"
    "    layout (location = 1) in vec3 color;\n"
    "    out vec3 ourColor;\n"
    "    uniform mat4 MVP;\n"
    "    void main() {\n"
    "        gl_Position = MVP * vec4(position.x, position.y, position.z, 1.0f);\n"
    "        ourColor = color;\n"
    "    }\0";        
const char *debugFragmentShader = "\n"
    "    #version 330 core\n"
    "    in vec3 ourColor;\n"
    "    out vec4 color;\n"
    "    void main() {\n"
    "        color = vec4(ourColor, 1.0f);\n"
    "    }\n\0";


namespace Tool {
    namespace Logger {
        enum Level {
            trace,
            debug,
            info,
            warning,
            error,
            critical
        };


        class Logger {
        private:
            Level _level;
            std::shared_ptr<spdlog::logger> _logger;

        public:
            Logger(const char *name) {
                this->_level = Level::info;
                this->_logger = spdlog::stdout_color_mt(name);
            }

            Level GetLevel() {
                return this->_level;
            }

            void SetLevel(Level level) {
                switch (level) {
                    case Level::trace:
                        this->_level = Level::trace;
                        this->_logger->set_level(spdlog::level::trace);
                        break;
                    case Level::debug:
                        this->_level = Level::debug;
                        this->_logger->set_level(spdlog::level::debug);
                        break;
                    case Level::info:
                        this->_level = Level::info;
                        this->_logger->set_level(spdlog::level::info);
                        break;
                    case Level::warning:
                        this->_level = Level::warning;
                        this->_logger->set_level(spdlog::level::warn);
                        break;
                    case Level::error:
                        this->_level = Level::error;
                        this->_logger->set_level(spdlog::level::err);
                        break;
                    case Level::critical:
                        this->_level = Level::critical;
                        this->_logger->set_level(spdlog::level::critical);
                        break;
                }
            }

            void trace(const char *message) {
                this->_logger->trace(message);
            }

            void debug(const char *message) {
                this->_logger->debug(message);
            }

            void info(const char *message) {
                this->_logger->info(message);
            }

            void warning(const char *message) {
                this->_logger->warn(message);
            }

            void error(const char *message) {
                this->_logger->error(message);
            }

            void critical(const char *message) {
                this->_logger->critical(message);
            }
        };
    }
}


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
                this->Position = glm::vec3(0.0f, 0.0f, 0.0f);
                this->WorldUp = glm::vec3(0.0f, 5.0f, 0.0f);
                this->Yaw = -90.0f;
                this->Pitch = 0.0f;
                this->MovementSpeed = 3.0f;
                this->MouseSensitivity = 0.25f;
                this->Zoom = 90.0f;

                this->updateCameraVectors();
            }

            virtual ~OpenglCamera() {

            }

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


    namespace Render {
        enum Shader {
            VERTEX,
            FRAGMENT
        };


        enum Backend {
            OPENGL,
        };


        class Buffer {
        public:
            uint32_t object;

            Buffer() {
                glGenBuffers(1, &this->object);
            }

            virtual ~Buffer() {
                glDeleteBuffers(1, &this->object);
            }

            void unbind() {
                glBindBuffer(GL_ARRAY_BUFFER, 0);
            }
        };


        // TODO: Add create method for buffer and overide with renderBackend
        class VertexBuffer : public Buffer {};


        class IndexBuffer : public Buffer {
        public:
            uint32_t count = 0;
        };


        class OpenglVertexBuffer : public VertexBuffer {
        public:
            void bind(float *vertices, uint32_t size) {
                glBindBuffer(GL_ARRAY_BUFFER, this->object);
                glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
            }
        };


        class OpenglIndexBuffer : public IndexBuffer {
        public:
            void bind(uint32_t *indices, uint32_t count) {
                this->count = count;

                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->object);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
            }
        };


        class Texture {
        public:
            uint32_t object;
        };


        class OpenglTexture : public Texture {
        public:
            OpenglTexture(const char *imagePath) {
                glGenTextures(1, &this->object);

                int width, height;

                glBindTexture(GL_TEXTURE_2D, this->object);

                unsigned char* image = SOIL_load_image(imagePath, &width, &height, 0, SOIL_LOAD_RGB);
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
                glGenerateMipmap(GL_TEXTURE_2D);
                SOIL_free_image_data(image);
                glBindTexture(GL_TEXTURE_2D, 0); 
            }

            virtual ~OpenglTexture() {
                glDeleteTextures(1, &this->object);
            }

            void bind() {
                glBindTexture(GL_TEXTURE_2D, this->object);
            }
        };

        class ShaderProgram {
        private:
            // std::unique_ptr<Logger> logger = std::make_unique<Logger>("shader");

        public:
            GLuint object;

            ShaderProgram(const char *vertexSource, const char *fragmentSource) {
                std::optional<GLuint> vertex = this->compile(
                    Shader::VERTEX, vertexSource
                );
                std::optional<GLuint> fragment = this->compile(
                    Shader::FRAGMENT, fragmentSource
                );

                this->object = glCreateProgram();

                if (vertex) {
                    glAttachShader(this->object, vertex.value());
                }
                else {
                    std::cout << "Shader compilation result not received" << std::endl;
                    // this->logger->warning("Shader compilation result not received");
                }

                if (fragment) {
                    glAttachShader(this->object, fragment.value());
                }
                else {
                    std::cout << "Shader compilation result not received" << std::endl;
                    // this->logger->warning("Shader compilation result not received");
                }

                this->linking();

                if (vertex) {
                    glDeleteShader(vertex.value());
                }

                if (fragment) {
                    glDeleteShader(fragment.value());
                }
            }

            ~ShaderProgram() {
                glDeleteProgram(this->object);
            }

            std::optional<GLuint> compile(Shader type, const char *file) {
                GLint compileStatus;
                GLuint shader;

                switch (type) {
                    case Shader::VERTEX:
                        shader = glCreateShader(GL_VERTEX_SHADER);
                        break;
                    case Shader::FRAGMENT:
                        shader = glCreateShader(GL_FRAGMENT_SHADER);
                        break;
                    default:
                        throw std::logic_error("Shader type is not implement");
                }

                glShaderSource(shader, 1, &file, NULL);
                glCompileShader(shader);
                glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);

                if (compileStatus) {
                    return shader;
                }
                else {
                    GLchar errorMessage[512];

                    glGetShaderInfoLog(shader, 512, NULL, errorMessage);

                    std::cout << fmt::format("Shader compile error {}\n{}", errorMessage, file).c_str() << std::endl;
                    // this->logger->error(
                        // fmt::format("Shader compile error {}\n{}", errorMessage, file).c_str()
                    // );

                    return std::nullopt;
                }
            }

            void linking() {
                GLint linkingStatus;

                glLinkProgram(this->object);
                glGetProgramiv(this->object, GL_LINK_STATUS, &linkingStatus);

                if (!linkingStatus) {
                    GLchar errorMessage[512];

                    glGetProgramInfoLog(this->object, 512, NULL, errorMessage);

                    std::cout << fmt::format("Shader program linking: {}", errorMessage).c_str() << std::endl;

                    // this->logger->error(
                        // fmt::format("Shader program linking: {}", errorMessage).c_str()
                    // );
                }
            }

            void use() {
                glUseProgram(this->object);
            }

            void uniformColor(const char *attribute, float r, float g, float b) {
                GLint color = glGetUniformLocation(this->object, attribute);
                glUniform3f(color, r, g, b);
            }

            void uniformPosition(const char *attribute, float x, float y, float z) {
                 GLint position = glGetUniformLocation(this->object, attribute);
                glUniform3f(position, x, y, z);
            }

            void uniformMatrix(const char *attribure, glm::mat4 matrix) {
                GLuint targetAttribute = glGetUniformLocation(this->object, attribure);
                glUniformMatrix4fv(targetAttribute, 1, GL_FALSE, glm::value_ptr(matrix));
            }
        };
    }


    namespace Debug {
        using namespace Engine::Render;


        class Debug {
            virtual void Enable() = 0;
        };

        class DebugAxes : public Debug {};


        class OpenglDebugAxes : public DebugAxes {
        public:
            GLuint VAO;
            glm::mat4 MVP;
            OpenglVertexBuffer *VBO = nullptr;
            ShaderProgram *shader = nullptr;

            OpenglDebugAxes() {
                this->shader = new Render::ShaderProgram(
                    debugVertexShader, debugFragmentShader
                );

                GLfloat vertices[] = {
                    0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                    1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                    0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
                    0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
                    0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
                    0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f
                };

                glGenVertexArrays(1, &this->VAO);
                this->VBO = new OpenglVertexBuffer();

                glBindVertexArray(this->VAO);
                this->VBO->bind(vertices, sizeof(vertices));

                // Position
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
                glEnableVertexAttribArray(0);
                
                // Color attribute
                glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
                glEnableVertexAttribArray(1);

                this->VBO->unbind();

                glBindVertexArray(0);
            }

            virtual ~OpenglDebugAxes() {
                glDeleteVertexArrays(1, &this->VAO);

                delete this->VBO;
                delete this->shader;
            }

            void SetMVP(glm::mat4 mvp) {
                this->MVP = mvp;
            }

            void Enable() {
                this->shader->use();
                this->shader->uniformMatrix("MVP", this->MVP);

                glBindVertexArray(this->VAO);
                // 6 is a count vertex
                glDrawArrays(GL_LINES, 0, 6);
                glBindVertexArray(0);
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
                logger->trace("Startup");

                glfwInit();

                glfwWindowHint(GLFW_SAMPLES, 4);

                glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
                glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
            }

            void Create() {
                logger->trace("Create");

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
                logger->trace("Shutdown");

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
        Render::Backend renderBackend = Render::Backend::OPENGL;
        Window::WindowProvider *window = nullptr;
        Editor::Editor *editor = nullptr;

        Application() {
            logger->trace("constructor");

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
            logger->trace("destructor");

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
            logger->trace("constructor");

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

            logger->info(fmt::format("engine version: {}", ENGINE_VERSION).c_str());
            logger->info(fmt::format("glsl version: {}", GLSL_VERSION).c_str());

            // Initialize all managers and then initialize the application
            // since the application can change the settings of managers.
            this->app->window->Startup();
            this->app->Startup();
        }

        void Run() {
            /**
             * Main engine loop.
             * 
             * As long as the application window is open, engine 
             * continues updating user application and all managers. 
             */

            logger->trace("run");

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

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

GLuint screenWidth = 1920, screenHeight = 1080;


// Moves/alters the camera positions based on user input
void Do_Movement()
{
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
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
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

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
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

    camera.ProcessMouseMovement(xoffset, yoffset);
}	


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    camera.ProcessMouseScroll(yoffset);
}


class UserApplication : public Engine::Application {
public:
    GLuint VAO;
    Engine::Render::OpenglVertexBuffer *VBO = nullptr;
    // Engine::Render::OpenglIndexBuffer *EBO = nullptr;
    Engine::Render::OpenglTexture *texture = nullptr;
    Engine::Render::ShaderProgram *shader = nullptr;

    Engine::Debug::OpenglDebugAxes *debugAxes = nullptr;

    void Startup() {
        logger->trace("Startup");

        this->window->settings = {
            screenWidth, screenHeight, false, true, "Application"
        };
    }

    void Run() {
        glfwSetKeyCallback(static_cast<Engine::Window::GLFWWindowProvider*>(this->window)->object, key_callback);
        glfwSetCursorPosCallback(static_cast<Engine::Window::GLFWWindowProvider*>(this->window)->object, mouse_callback);
        glfwSetScrollCallback(static_cast<Engine::Window::GLFWWindowProvider*>(this->window)->object, scroll_callback);

        this->shader = new Engine::Render::ShaderProgram(
            vertexShaderSource, fragmentShaderSource
        );
        this->debugAxes = new Engine::Debug::OpenglDebugAxes();

        this->texture = new Engine::Render::OpenglTexture(
            "/home/a-kletsko/Projects/engine/container.jpg"
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

        glGenVertexArrays(1, &this->VAO);
        this->VBO = new Engine::Render::OpenglVertexBuffer();
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

        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        Do_Movement();

        glm::mat4 view;
        view = camera.GetViewMatrix();
        glm::mat4 projection;	
        projection = glm::perspective(glm::radians(camera.Zoom), (float)screenWidth/(float)screenHeight, 0.1f, 1000.0f);

        this->shader->use();
        this->shader->uniformColor("testColor", color[0], color[1], color[2]);
        this->shader->uniformPosition("ourPosition", position[0], position[1], position[2]);

        this->shader->uniformMatrix("view", view);
        this->shader->uniformMatrix("projection", projection);

        this->texture->bind(); 

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
            this->shader->uniformMatrix("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        glBindVertexArray(0);

        // TODO: Move to the editor as debug flag
        glm::mat4 mvp(1.0f);
        this->debugAxes->SetMVP(view);
        this->debugAxes->Enable();
    }

    void Shutdown() {
        logger->trace("Shutdown");

        glDeleteVertexArrays(1, &this->VAO);

        delete shader;
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
