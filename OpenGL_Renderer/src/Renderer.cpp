#include "pch.h"
#include "Renderer.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "ErrorCheck.h"

GLFWwindow* window;
const unsigned int frameWidth = 1920;
const unsigned int frameHeight = 1080;
const unsigned char aaSamples = 32;

glm::vec3 cameraPos = glm::vec3(0.0f, 2.0f, 6.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 0.5f, 0.0f);

bool firstMouse = true;
float yaw = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch = 0.0f;
float lastX = 800.0f / 2.0;
float lastY = 600.0f / 2.0;
float fov = 45.0f;

float deltaTime = 0.0f;
float lastFrame = 0.0f;
unsigned int timeCounter = 0;
float cameraSpeed = 2.0f;

Renderer::Renderer()
{
    
}

Renderer::~Renderer()
{
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    //float cameraSpeed = 2.0f * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    {
        cameraSpeed = 8.0f * deltaTime;
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            cameraPos += cameraSpeed * cameraFront;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            cameraPos -= cameraSpeed * cameraFront;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
            cameraPos += cameraSpeed * cameraUp;
        if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
            cameraPos -= cameraSpeed * cameraUp;
    }
    else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
    {
        cameraSpeed = 2.0f * deltaTime;
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            cameraPos += cameraSpeed * cameraFront;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            cameraPos -= cameraSpeed * cameraFront;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
            cameraPos += cameraSpeed * cameraUp;
        if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
            cameraPos -= cameraSpeed * cameraUp;
    }

}
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(direction);
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    fov -= (float)yoffset;
    if (fov < 1.0f)
        fov = 1.0f;
    if (fov > 45.0f)
        fov = 45.0f;
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

bool Renderer::Init()
{   
    /* Initialize the library */
    if (!glfwInit()) return false;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(frameWidth, frameHeight, "OpenGL", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return false;
    }
    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (glewInit() != GLEW_OK) {
        std::cout << "Glew init error" << std::endl;
    }

    GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));   
    GLCall(glEnable(GL_DEPTH_TEST));
    GLCall(glEnable(GL_BLEND));
    GLCall(glEnable(GL_CULL_FACE));
    GLCall(glEnable(GL_MULTISAMPLE));
    GLCall(glCullFace(GL_BACK));
    std::cout << glGetString(GL_VERSION) << std::endl;
    
}

int Renderer::GetWidth()
{
    return frameWidth;
}

int Renderer::GetHeight()
{
    return frameHeight;
}

int Renderer::GetAASamples()
{
    return aaSamples;
}

glm::vec3 Renderer::GetCameraPos()
{
    return cameraPos;
}
glm::vec3 Renderer::GetCameraFront()
{
    return cameraFront;
}
glm::vec3 Renderer::GetCameraUp()
{
    return cameraUp;
}
float Renderer::GetFov()
{
    return fov;
}

bool Renderer::IsWindowClosed()
{
    if (!glfwWindowShouldClose(window)) return true;
    else 
        return false;
}

void Renderer::Terminate()
{
    glfwTerminate();
}

void Renderer::SwapBuffers()
{
    /* Swap front and back buffers */
    glfwSwapBuffers(window);
    /* Poll for and process events */
    glfwPollEvents();
}

void Renderer::ProceedInput()
{
    //WASD input
    double currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    timeCounter++;
    //Show fps count
    if (deltaTime >= 1.0 / 30.0)
    {
        // Creates new title
        std::string FPS = std::to_string((1.0 / deltaTime) * timeCounter);
        std::string ms = std::to_string((deltaTime / timeCounter) * 1000);
        std::string newTitle = "OpenGL - " + FPS + " FPS / " + ms + " ms";
        glfwSetWindowTitle(window, newTitle.c_str());

        // Resets times and counter
        lastFrame = currentFrame;
        timeCounter = 0;
    }
    processInput(window);
}

void Renderer::Clear(glm::vec4 color){
    GLCall(glClearColor(color.r, color.g, color.b, color.a));
    GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));   
}

void Renderer::DrawIB(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const {
    shader.Bind();
    va.Bind();
    ib.Bind();
    GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::DrawVB(const VertexArray& va, const VertexBuffer& vb, const Shader& shader) const {
    shader.Bind();
    va.Bind();
    GLCall(glDrawArrays(GL_TRIANGLES, 0, vb.GetSize()/sizeof(float)));
}


