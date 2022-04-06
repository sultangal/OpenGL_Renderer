#include "pch.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Renderer.h"
#include "ObjImporter.h"
#include "Texture.h"
#include "FrameBuffer.h"
#include "RenderBuffer.h"

const unsigned int frameWidth = 2560;
const unsigned int frameHeight = 1440;
const unsigned char aaSamples = 16;

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

int main(void)
{
    GLFWwindow* window;
    /* Initialize the library */
    if (!glfwInit())
        return -1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(frameWidth, frameHeight, "OpenGL", NULL, NULL);
    if (!window){
        glfwTerminate();
        return -1;
    }
    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (glewInit() != GLEW_OK){
        std::cout << "Glew init error" << std::endl;
    }   
    GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
    GLCall(glEnable(GL_DEPTH_TEST));
    GLCall(glEnable(GL_BLEND));   
    GLCall(glEnable(GL_CULL_FACE));
    GLCall(glCullFace(GL_BACK));   
    std::cout << glGetString(GL_VERSION) << std::endl;
    {   
                FrameBuffer fb;
                // create a multisampled color attachment texture
                Texture textureColorBufferMultiSampled(frameWidth, frameHeight, aaSamples, 0);
                // create a (also multisampled) renderbuffer object for depth and stencil attachments
                RenderBuffer rb(frameWidth, frameHeight, aaSamples);

                fb.CheckComplitness();
                fb.BindDefaultFB();

                // configure second post-processing framebuffer
                FrameBuffer interFb;
                // create a color attachment texture
                Texture screenTexture(frameWidth, frameHeight, 2);

                interFb.CheckComplitness();
                fb.BindDefaultFB();

                float quadVertices[] = {   // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
                    // positions   // texCoords
                    -1.0f,  1.0f,  0.0f, 1.0f,
                    -1.0f, -1.0f,  0.0f, 0.0f,
                     1.0f, -1.0f,  1.0f, 0.0f,

                    -1.0f,  1.0f,  0.0f, 1.0f,
                     1.0f, -1.0f,  1.0f, 0.0f,
                     1.0f,  1.0f,  1.0f, 1.0f
                };

        ObjImporter objImport("res/models/statue_hard2.obj", true);

        //vertex array generation
        VertexArray va;

        //vertex buffer generation
        VertexBuffer vb(objImport.GetVertecies(), objImport.GetVertCount());
       
        //attributes generation
        VertexBufferLayout layout;
        layout.Push<float>(3); layout.Push<float>(3); layout.Push<float>(2); layout.Push<float>(3);  
        VertexBufferLayout layoutTBN;
        layoutTBN.Push<float>(3); layoutTBN.Push<float>(3); layoutTBN.Push<float>(2); layoutTBN.Push<float>(3); layoutTBN.Push<float>(3); layoutTBN.Push<float>(3);
        va.AddBuffer(vb, layoutTBN);
        //index buffer generation
        //IndexBuffer ib(objImport.GetIndecies(), objImport.GetIndeciesCount());                        

        ObjImporter objImport2("res/models/cube.obj", false);
        VertexArray va2;
        VertexBuffer vb2(objImport2.GetVertecies(), objImport2.GetVertCount());      
        va2.AddBuffer(vb2, layout);

        ObjImporter objImport3("res/models/statue_hard2.obj", true);
        VertexArray va3;
        VertexBuffer vb3(objImport3.GetVertecies(), objImport3.GetVertCount());
        va3.AddBuffer(vb3, layoutTBN);
        
        //shader generation
        Shader shader("res/shaders/Vertex.vert", "res/shaders/Fragment.frag");
        Shader normalShader("res/shaders/NormalMap.vert", "res/shaders/NormalMap.frag");
        Shader shader2("res/shaders/Color.vert", "res/shaders/Color.frag");
        Shader shaderFB("res/shaders/FrameBuffer.vert", "res/shaders/FrameBuffer.frag");

        Renderer renderer;

        //texture generation
        Texture tex001("res/textures/tgziabifa_4K_Albedo.jpg", 0, true);
        Texture tex002("res/textures/tgziabifa_4K_Specular.jpg", 1, true);
        Texture tex004("res/textures/tgziabifa_4K_Normal_LOD0.jpg", 4, false);
        Texture tex003("res/textures/tgziabifa_4K_Albedo.jpg", 3, true);      
        Texture tex005("res/textures/tgziabifa_4K_Specular.jpg", 5, true);      
        Texture tex006("res/textures/tgziabifa_4K_Normal_LOD0.jpg", 6, false);

        glm::vec3 positions[10] =
        {{3.0f, 1.0f, -1.0f},
         {0.0f, 0.0f, 1.0f},
         {1.0f, 0.0f, 3.0f},
         {2.0f, 0.0f, 3.0f},
         {2.0f, 1.0f, 3.0f},
         {0.0f, 4.0f, 0.0f},
         {2.0f, 0.0f, 2.0f},
         {1.0f, 1.0f, -1.0f},
         {2.0f, 3.0f, 0.0f},
         {1.0f, 2.0f, 0.0f}};       
        float rot = 0.0f;    
        //glm::vec4 environmentColor = glm::vec4(0.5f, 0.85f, 5.0f, 1.0f);
        glm::vec4 environmentColor = glm::vec4(0.5f, 1.0f, 3.0f, 1.0f);
        
        //MAIN LOOP
        while (!glfwWindowShouldClose(window))
        {
            //WASD input
            float currentFrame = glfwGetTime();
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

                // Use this if you have disabled VSync
                //camera.Inputs(window);
            }          
            processInput(window);

                    // draw scene as normal in multisampled buffers
                    fb.Bind();
                    renderer.Clear(environmentColor);

            /* Render here */           
            glm::mat4 view = glm::mat4(1.0f);
            view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
            glm::mat4 projection = glm::mat4(1.0f);
            projection = glm::perspective(glm::radians(fov), (float)frameWidth / (float)frameHeight, 0.1f, 100.0f);
            glm::mat4 model = glm::mat4(1.0f);

            glm::vec3 lightPosition = { 0.0f, 3.0f, 0.0f };
            glm::vec3 lightDirection = { 0.0f, 0.0f, 0.0f };
            glm::vec3 lightColor = { 10.0f, 9.0f, 7.0f };
            //glm::vec3 lightColor = { 0.0f, 0.0f, 0.0f };
            glm::vec3 ambientLight = { 0.2f, 0.2f, 0.2f };
            
            ////////////////////////////////////OBJECTS     
            for (unsigned char i = 0; i < 10; i++)
            {
                normalShader.Bind();
                normalShader.SetUniform1i("diffuseMap", tex003.GetTexSlothID());
                normalShader.SetUniform1i("specularTexture", tex005.GetTexSlothID());
                normalShader.SetUniform1i("normalMap", tex006.GetTexSlothID());
                normalShader.SetUniformMatrix4fv("view", glm::value_ptr(view));
                normalShader.SetUniformMatrix4fv("projection", glm::value_ptr(projection));
                model = glm::mat4(1.0f);               
                model = glm::translate(model, positions[i]*4.3f);      
                model = glm::rotate(model, glm::radians(i*20.0f), glm::vec3(1.0f, 0.0f, 0.0f));
                model = glm::scale(model, glm::vec3(0.5f));
                normalShader.SetUniformMatrix4fv("model", glm::value_ptr(model));
                //glm::vec3 u_Color = { 1.0f, 1.0f, 0.0f };
                //shader.SetUniform3f("u_Color", u_Color.x, u_Color.y, u_Color.z);               
                normalShader.SetUniform3f("lightPosition", lightPosition.x, lightPosition.y, lightPosition.z);
                //shader.SetUniform3f("directionalLight", -0.2f, -1.0f, -0.3f);                
                normalShader.SetUniform3f("lightColor", lightColor.x, lightColor.y, lightColor.z);
                normalShader.SetUniform3f("ambientLight", ambientLight.x, ambientLight.y, ambientLight.z);
                normalShader.SetUniform3f("viewPos", cameraPos.x, cameraPos.y, cameraPos.z);
                normalShader.SetUniform1f("constant", 1.0f);
                normalShader.SetUniform1f("linear", 0.09f);
                normalShader.SetUniform1f("quadratic", 0.032f);

                //spotlight
                //shader.SetUniform3f("spotlight.position", lightPosition.x, lightPosition.y, lightPosition.z );
                //shader.SetUniform3f("spotlight.direction", lightDirection.x, lightDirection.y, lightDirection.z);
                //shader.SetUniform1f("spotlight.cutOff", glm::cos(glm::radians(21.5f)));
                //shader.SetUniform1f("spotlight.outerCutOff", glm::cos(glm::radians(22.5f)));

                normalShader.SetUniform1f("constant", 1.0f);
                normalShader.SetUniform1f("linear", 0.09f);
                normalShader.SetUniform1f("quadratic", 0.032f);

                renderer.DrawVB(va, vb, normalShader);
            }
            ////////////////////////////////////LIGHT_SOURCE
            shader2.Bind();
            shader2.SetUniformMatrix4fv("view", glm::value_ptr(view));
            shader2.SetUniformMatrix4fv("projection", glm::value_ptr(projection));
            shader2.SetUniform3f("lightColor", lightColor.x, lightColor.y, lightColor.z);
            model = glm::mat4(1.0f);             
            model = glm::translate(model, { lightPosition.x, lightPosition.y, lightPosition.z });
            model = glm::scale(model, glm::vec3(0.5f));
            shader2.SetUniformMatrix4fv("model", glm::value_ptr(model)); 
            renderer.DrawVB(va2, vb2, shader2);

            ////////////////////////////////////PLANE
           normalShader.Bind();
           normalShader.SetUniform1i("diffuseMap", tex001.GetTexSlothID());
           normalShader.SetUniform1i("specularTexture", tex002.GetTexSlothID());
           normalShader.SetUniform1i("normalMap", tex004.GetTexSlothID());
           model = glm::mat4(1.0f);
           model = glm::translate(model, glm::vec3(0.0f, 0.0f, -10.0f));
           model = glm::rotate(model, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));
           model = glm::scale(model, glm::vec3(1.0f));
           
           normalShader.SetUniformMatrix4fv("model", glm::value_ptr(model));
           normalShader.SetUniformMatrix4fv("view", glm::value_ptr(view));
           normalShader.SetUniformMatrix4fv("projection", glm::value_ptr(projection));                     
           normalShader.SetUniform3f("viewPos", cameraPos.x, cameraPos.y, cameraPos.z);
           normalShader.SetUniform3f("lightPos", lightPosition.x, lightPosition.y, lightPosition.z);
           normalShader.SetUniform3f("lightColor", lightColor.x, lightColor.y, lightColor.z);
           normalShader.SetUniform3f("ambientLight", ambientLight.x, ambientLight.y, ambientLight.z);
           normalShader.SetUniform1f("constant", 1.0f);
           normalShader.SetUniform1f("linear", 0.09f);
           normalShader.SetUniform1f("quadratic", 0.032f);
           renderer.DrawVB(va3, vb3, normalShader);
            /////////////////////////////////////////////////

                    //blit multisampled buffer(s) to normal colorbuffer of intermediate FBO. Image is stored in screenTexture
                    fb.BindRead();
                    interFb.BindDraw();
                    interFb.Blit(frameWidth, frameHeight, aaSamples);
                    interFb.BindDefaultFB();
                    renderer.Clear(glm::vec4(1.0f));

                    //draw screen quad
                    VertexBuffer vbFB(quadVertices, 24 * sizeof(float));
                    VertexBufferLayout layoutFB;
                    layoutFB.Push<float>(2); layoutFB.Push<float>(2);
                    VertexArray vaFB;
                    vaFB.AddBuffer(vbFB, layoutFB);
                    
                    shaderFB.Bind();
                    screenTexture.Bind();
                    shaderFB.SetUniform1i("screenTexture", screenTexture.GetTexSlothID());                  
                    renderer.DrawVB(vaFB, vbFB, shaderFB);

            /* Swap front and back buffers */
            glfwSwapBuffers(window);
            /* Poll for and process events */
            glfwPollEvents();
            rot += 0.05f;
        }
    }
    glfwTerminate();
    return 0;
}