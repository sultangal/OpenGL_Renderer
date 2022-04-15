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

Renderer renderer;
int frameWidth = renderer.GetWidth();
int frameHeight = renderer.GetHeight();
int aaSamples = renderer.GetAASamples();

int main(void)
{   
    if (!renderer.Init()) return -1;       
    {   
        FrameBuffer fb;
        Texture textureColorBufferMultiSampled(frameWidth, frameHeight, aaSamples, 0);        
        RenderBuffer rb(frameWidth, frameHeight, aaSamples);
        fb.CheckComplitness();
        fb.Unbind();

        FrameBuffer interFb;
        Texture screenTexture(frameWidth, frameHeight, 2);
        interFb.CheckComplitness();
        interFb.Unbind();

        float quadVertices[] = {   // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
            // positions   // texCoords
            -1.0f,  1.0f,  0.0f, 1.0f,
            -1.0f, -1.0f,  0.0f, 0.0f,
             1.0f, -1.0f,  1.0f, 0.0f,

            -1.0f,  1.0f,  0.0f, 1.0f,
             1.0f, -1.0f,  1.0f, 0.0f,
             1.0f,  1.0f,  1.0f, 1.0f
        };

        //draw screen quad
        VertexBuffer vbFB(quadVertices, 24 * sizeof(float));
        VertexBufferLayout layoutFB;
        layoutFB.Push<float>(2); layoutFB.Push<float>(2);
        VertexArray vaFB;
        vaFB.AddBuffer(vbFB, layoutFB);

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
        Shader toneMappingShader("res/shaders/ToneMapping.vert", "res/shaders/ToneMapping.frag");
        Shader normalShader("res/shaders/NormalMap.vert", "res/shaders/NormalMap.frag");
        Shader pureColor("res/shaders/Color.vert", "res/shaders/Color.frag");
        Shader shaderFB("res/shaders/FrameBuffer.vert", "res/shaders/FrameBuffer.frag");      

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
        glm::vec3 cameraPos = glm::vec3();
        glm::vec3 cameraFront = glm::vec3();
        glm::vec3 cameraUp = glm::vec3();
        float fov = 0.0f;
        while (renderer.IsWindowClosed())
        {
            renderer.ProceedInput();
            cameraPos = renderer.GetCameraPos();
            cameraFront = renderer.GetCameraFront();
            cameraUp = renderer.GetCameraUp();
            fov = renderer.GetFov();
            
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
            /*for (unsigned char i = 0; i < 10; i++)
            {
                normalShader.Bind();
                normalShader.SetUniform1i("diffuseMap", tex003.GetTexSlotID());
                normalShader.SetUniform1i("specularTexture", tex005.GetTexSlotID());
                normalShader.SetUniform1i("normalMap", tex006.GetTexSlotID());
                normalShader.SetUniformMatrix4fv("view", glm::value_ptr(view));
                normalShader.SetUniformMatrix4fv("projection", glm::value_ptr(projection));
                model = glm::mat4(1.0f);               
                model = glm::translate(model, positions[i]*4.3f);      
                model = glm::rotate(model, glm::radians(i*20.0f), glm::vec3(1.0f, 0.0f, 0.0f));
                model = glm::scale(model, glm::vec3(0.5f));
                normalShader.SetUniformMatrix4fv("model", glm::value_ptr(model));
                //glm::vec3 u_Color = { 1.0f, 1.0f, 0.0f };
                //shader.SetUniform3f("u_Color", u_Color.x, u_Color.y, u_Color.z);               
                //normalShader.SetUniform3f("lightPosition", lightPosition.x, lightPosition.y, lightPosition.z);
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
            }*/
            ////////////////////////////////////LIGHT_SOURCE
            pureColor.Bind();
            pureColor.SetUniformMatrix4fv("view", glm::value_ptr(view));
            pureColor.SetUniformMatrix4fv("projection", glm::value_ptr(projection));
            pureColor.SetUniform3f("lightColor", lightColor.x, lightColor.y, lightColor.z);
            model = glm::mat4(1.0f);             
            model = glm::translate(model, { lightPosition.x, lightPosition.y, lightPosition.z });
            model = glm::scale(model, glm::vec3(0.5f));
            pureColor.SetUniformMatrix4fv("model", glm::value_ptr(model)); 
            renderer.DrawVB(va2, vb2, pureColor);
            
            ////////////////////////////////////
            normalShader.Bind();
            normalShader.SetUniform1i("diffuseMap", tex001.GetTexSlotID());
            normalShader.SetUniform1i("specularTexture", tex002.GetTexSlotID());
            normalShader.SetUniform1i("normalMap", tex004.GetTexSlotID());
            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(0.0f, 0.0f, -10.0f));
            model = glm::rotate(model, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));
            rot += 0.05f;
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
            interFb.BindDraw();
            interFb.Blit(frameWidth, frameHeight);
            interFb.Unbind();
            renderer.Clear(glm::vec4(1.0f));
            toneMappingShader.Bind();
            toneMappingShader.SetUniform1i("screenTexture", screenTexture.GetTexSlotID());
            renderer.DrawVB(vaFB, vbFB, toneMappingShader);           

            //blit multisampled buffer(s) to normal colorbuffer of intermediate FBO. Image is stored in screenTexture
            bool horizontal = true;
            for (unsigned int i = 0; i < 500; i++)
            {
                interFb.BindDraw();
                interFb.Blit(frameWidth, frameHeight);
                interFb.Unbind();
                renderer.Clear(glm::vec4(1.0f));            
                shaderFB.Bind();
                shaderFB.SetUniform1i("screenTexture", screenTexture.GetTexSlotID());
                shaderFB.SetUniform1i("horizontal", horizontal);
                renderer.DrawVB(vaFB, vbFB, shaderFB);
                horizontal = !horizontal;
            }



            renderer.SwapBuffers();        
        }
    }
    renderer.Terminate();
}