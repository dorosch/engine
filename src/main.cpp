#include <optional>
#include <functional>
#include <memory>
#include <iostream>
#include <fmt/format.h>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include "meta.hpp"
#include "editor.hpp"


const GLchar* vertexShaderSource = "\n    #version 330 core\n"
    "    layout (location = 0) in vec3 position;\n"
    "    uniform vec3 ourPosition;"
    "    void main() {\n"
    "        gl_Position = vec4(position.x + ourPosition.x, position.y + ourPosition.y, position.z + ourPosition.z, 1.0);\n"
    "    }\n\0";
const GLchar* fragmentShaderSource = "\n    #version 330 core\n"
    "    out vec4 color;\n"
    "    uniform vec4 ourColor;\n"
    "    void main() {\n"
    "        color = ourColor;\n"
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


        class ShaderProgram {
        private:
            std::unique_ptr<Logger> logger = std::make_unique<Logger>("shader");

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
                    this->logger->warning("Shader compilation result not received");
                }

                if (fragment) {
                    glAttachShader(this->object, fragment.value());
                }
                else {
                    this->logger->warning("Shader compilation result not received");
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

                    this->logger->error(
                        fmt::format("Shader compile error {}\n{}", errorMessage, file).c_str()
                    );

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

                    this->logger->error(
                        fmt::format("Shader program linking: {}", errorMessage).c_str()
                    );
                }
            }

            void use() {
                glUseProgram(this->object);
            }

            void uniformColor(const char *attribute, float r, float g, float b, float a) {
                GLint color = glGetUniformLocation(this->object, attribute);
                glUniform4f(color, r, g, b, a);
            }

            void uniformPosition(const char *attribute, float x, float y, float z) {
                 GLint position = glGetUniformLocation(this->object, attribute);
                glUniform3f(position, x, y, z);
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

            glewExperimental = GL_TRUE;
            glewInit();

            this->app->Run();

            while (this->app->window->IsOpen()) {
                glfwPollEvents();

                glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT);

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


class UserApplication : public Engine::Application {
public:
    GLuint VAO;
    Engine::Render::OpenglVertexBuffer *VBO = nullptr;
    Engine::Render::OpenglIndexBuffer *EBO = nullptr;
    Engine::Render::ShaderProgram *shader = nullptr;

    void Startup() {
        logger->trace("Startup");

        this->window->settings = {
            800, 600, false, true, "Application"
        };
    }

    void Run() {
        this->shader = new Engine::Render::ShaderProgram(
            vertexShaderSource, fragmentShaderSource
        );

        float vertices[] = {
            0.5f,  0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            -0.5f, -0.5f, 0.0f,
            -0.5f,  0.5f, 0.0f,
            0.6f, 0.75f, 0.0f
        };
        uint32_t indices[] = {
            0, 1, 3,
            1, 2, 3,
            0, 1, 2,
            4, 1, 2
        };

        glGenVertexArrays(1, &this->VAO);
        this->VBO = new Engine::Render::OpenglVertexBuffer();
        this->EBO = new Engine::Render::OpenglIndexBuffer();

        glBindVertexArray(VAO);
        this->VBO->bind(vertices, sizeof(vertices));
        this->EBO->bind(indices, sizeof(indices) / sizeof(uint32_t));

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);

        this->VBO->unbind();

        glBindVertexArray(0);
    }

    void Update() {
        this->shader->use();
        this->shader->uniformColor("ourColor", color[0], color[1], color[2], color[3]);
        this->shader->uniformPosition("ourPosition", position[0], position[1], position[2]);

        glBindVertexArray(this->VAO);
        glDrawElements(GL_TRIANGLES, this->EBO->count, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    void Shutdown() {
        logger->trace("Shutdown");

        glDeleteVertexArrays(1, &this->VAO);

        delete shader;
        delete VBO;
        delete EBO;
    }
};


int main() {
    UserApplication *application = new UserApplication();
    Engine::Engine engine(application);

    engine.Startup();

    try {
        engine.Run();
    } catch (const std::exception& error) {
        delete application;

        return EXIT_FAILURE;
    }

    delete application;

    return EXIT_SUCCESS;
}
