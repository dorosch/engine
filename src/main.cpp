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
// #include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "engine.hpp"
#include "app.hpp"
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
#include "core/scene/scene.hpp"
#include "core/window/base.hpp"
#include "core/window/glfw.hpp"


namespace Engine {
    using namespace Tool::Logger;


    namespace Scene {
        enum CameraDirection {
            FORWARD,
            BACKWARD,
            LEFT,
            RIGHT
        };


        class Camera {};


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

    // For keyboard io.WantCaptureKeyboard
    if (io.WantCaptureMouse == false) {

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

    ImGuiIO& io = ImGui::GetIO();

    if (io.WantCaptureMouse == false) {
        camera.ProcessMouseScroll(yoffset);
    }
}


class UserApplication : public Engine::EngineApplication {
public:
    GLuint VAO, modelVAO;
    Engine::Render::VertexBuffer *VBO = nullptr;
    Engine::Render::VertexBuffer *modelVBO = nullptr;
    // Engine::Render::IndexBuffer *EBO = nullptr;
    Engine::Render::ShaderProgram *shader = nullptr;
    Engine::Render::ShaderProgram *shaderModel = nullptr;
    Engine::Render::Texture *texture = nullptr;
    Engine::Render::Texture *modelTexture = nullptr;

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

        GLFWwindow *window = static_cast<Engine::Window::GLFWWindowProvider*>(this->window)->object;
        glfwSetMouseButtonCallback(window, mouse_button_callback);
        glfwSetCursorPosCallback(window, cursor_position_callback);
        glfwSetKeyCallback(window, key_callback);
        glfwSetCursorPosCallback(window, mouse_callback);
        glfwSetScrollCallback(window, scroll_callback);

        std::filesystem::path cwd = std::filesystem::current_path();

        this->model = new Tool::ObjModel(cwd / "resources" / "models" / "Old House" / "model.obj");
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

        this->modelTexture = Engine::Render::Texture::GetInstance();
        this->modelTexture->Build(
            cwd / "resources" / "models" / "Old House" / "textures" / "body.jpg"
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

        std::vector<float> data = this->model->Data();

        // Draw model
        glGenVertexArrays(1, &this->modelVAO);
        this->modelVBO = Engine::Render::VertexBuffer::GetInstance();
        // this->EBO = Engine::Render::IndexBuffer::GetInstance();

        glBindVertexArray(modelVAO);
        glBindBuffer(GL_ARRAY_BUFFER, this->modelVBO->object);
        glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), data.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);

        this->modelVBO->unbind();

        glBindVertexArray(0);
        // End draw model


        glGenVertexArrays(1, &this->VAO);
        this->VBO = Engine::Render::VertexBuffer::GetInstance();

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
        Do_Movement();

        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

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

        this->modelTexture->Bind();
        
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
