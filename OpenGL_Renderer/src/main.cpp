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
        //----------------------------//
        FrameBuffer mainFB;
        Texture textureMainFB(frameWidth, frameHeight, aaSamples, 8);
        RenderBuffer mainRB(frameWidth, frameHeight, aaSamples);
        mainFB.CheckComplitness();
        mainFB.Unbind();

        FrameBuffer mainPostFB;
        Texture mainPostFBTexture(frameWidth, frameHeight, 9);
        mainPostFB.CheckComplitness();
        mainPostFB.Unbind();

        FrameBuffer screenFB;
        Texture screenFBTexture(frameWidth, frameHeight, 2);
        screenFB.CheckComplitness();
        screenFB.Unbind();

        FrameBuffer gaussBlurFB;
        Texture textureGaussBlurFB(frameWidth, frameHeight, 7);
        gaussBlurFB.CheckComplitness();
        gaussBlurFB.Unbind();    
       
        //screen quad
        float quadVertices[] = {   // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
            // positions   // texCoords
            -1.0f,  1.0f,  0.0f, 1.0f,
            -1.0f, -1.0f,  0.0f, 0.0f,
             1.0f, -1.0f,  1.0f, 0.0f,
                     
            -1.0f,  1.0f,  0.0f, 1.0f,
             1.0f, -1.0f,  1.0f, 0.0f,
             1.0f,  1.0f,  1.0f, 1.0f
        };       
        VertexBuffer vbFB(quadVertices, 24 * sizeof(float));
        VertexBufferLayout layoutFB;
        layoutFB.Push<float>(2); layoutFB.Push<float>(2);
        VertexArray vaFB;
        vaFB.AddBuffer(vbFB, layoutFB);
        //----------------------------//
        
        //models import and vertex buffer, vertex array, layout generation for that models
        //ObjImporter objImport("res/models/statue_hard2.obj", true);
        //VertexArray va;
        //VertexBuffer vb(objImport.GetVertecies(), objImport.GetVertCount());       
        VertexBufferLayout layoutTBN;
        layoutTBN.Push<float>(3); layoutTBN.Push<float>(3); layoutTBN.Push<float>(2); layoutTBN.Push<float>(3); layoutTBN.Push<float>(3); layoutTBN.Push<float>(3);
        //va.AddBuffer(vb, layoutTBN);                        
        
        ObjImporter objImport2("res/models/cube.obj", false);
        VertexArray cubeVA;
        VertexBuffer cubeVB(objImport2.GetVertecies(), objImport2.GetVertCount()); 
        VertexBufferLayout cubeLayout;
        cubeLayout.Push<float>(3); cubeLayout.Push<float>(3); cubeLayout.Push<float>(2); cubeLayout.Push<float>(3);
        cubeVA.AddBuffer(cubeVB, cubeLayout);

        ObjImporter objImport3("res/models/statue_hard3.obj", true);
        VertexArray statueVA;
        VertexBuffer statueVB(objImport3.GetVertecies(), objImport3.GetVertCount());
        statueVA.AddBuffer(statueVB, layoutTBN);
        
        float skyboxVertices[] = {
            // positions          
            -1.0f,  1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f,  1.0f, -1.0f,


            -1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            -1.0f,  1.0f, -1.0f,
             1.0f,  1.0f, -1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
             1.0f, -1.0f,  1.0f
        };
        VertexArray skyboxVA;
        VertexBuffer skyboxVB(skyboxVertices, sizeof(skyboxVertices));
        VertexBufferLayout skyboxLayout;
        skyboxLayout.Push<float>(3);
        skyboxVA.AddBuffer(skyboxVB, skyboxLayout);
        
        //shader generation
        //Shader shader("res/shaders/Vertex.vert", "res/shaders/Fragment.frag");
        Shader toneMappingShader("res/shaders/ToneMapping.vert", "res/shaders/ToneMapping.frag");
        Shader normalShader("res/shaders/NormalMap.vert", "res/shaders/NormalMap.frag");
        Shader pureColor("res/shaders/Color.vert", "res/shaders/Color.frag");
        Shader gaussianBlurShader("res/shaders/GaussianBlur.vert", "res/shaders/GaussianBlur.frag");      
        Shader blendShader("res/shaders/Blend.vert", "res/shaders/Blend.frag");      
        Shader thresholdShader("res/shaders/Threshold.vert", "res/shaders/Threshold.frag");
        Shader skyboxShader("res/shaders/Skybox.vert", "res/shaders/Skybox.frag");
        Shader envMapShader("res/shaders/EnvironmentMap.vert", "res/shaders/EnvironmentMap.frag");
        
        //texture generation  
        //Texture tex001("res/textures/tgziabifa_4K_Albedo.jpg", 0, true);
        //Texture tex002("res/textures/tgziabifa_4K_Specular.jpg", 1, true);
        Texture tex004("res/textures/tgziabifa_4K_Normal_LOD0.jpg", 4, false);
        //Texture tex003("res/textures/tgziabifa_4K_Albedo.jpg", 3, true);       
        //Texture tex005("res/textures/tgziabifa_4K_Specular.jpg", 5, true);
        //Texture tex006("res/textures/tgziabifa_4K_Normal_LOD0.jpg", 6, false);
        std::string faces[6] = 
        {
                "res/textures/px.hdr",
                "res/textures/nx.hdr",
                "res/textures/py.hdr",
                "res/textures/ny.hdr",
                "res/textures/pz.hdr",
                "res/textures/nz.hdr"
        };
        Texture skyboxTexture(faces, 10);
        
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
        float rotation = 0.0f;    
        glm::vec4 environmentColor = glm::vec4(0.5f, 1.0f, 3.0f, 1.0f);                
                
        glm::vec3 cameraPos = glm::vec3();
        glm::vec3 cameraFront = glm::vec3();
        glm::vec3 cameraUp = glm::vec3();
        float fov = 0.0f;
        //MAIN LOOP
        while (renderer.IsWindowClosed())
        {
            renderer.ProceedInput();
            cameraPos = renderer.GetCameraPos();
            cameraFront = renderer.GetCameraFront();
            cameraUp = renderer.GetCameraUp();
            fov = renderer.GetFov();
            mainFB.Bind();                    
            renderer.Clear(environmentColor);
            mainFB.EnableDepthTest();
            /* Render here */           
            glm::mat4 view = glm::mat4(1.0f);
            view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
            glm::mat4 projection = glm::mat4(1.0f);
            projection = glm::perspective(glm::radians(fov), (float)frameWidth / (float)frameHeight, 0.1f, 100.0f);
            glm::mat4 model = glm::mat4(1.0f);
            
            glm::vec3 lightPosition = { 0.0f, 3.0f, 0.0f };
            glm::vec3 lightDirection = { 0.0f, 0.0f, 0.0f };
            glm::vec3 lightColor = { 10.0f, 9.0f, 7.0f };
            //glm::vec3 lightColor = { 4.5f, 1.1f, 0.1f };
            glm::vec3 ambientLight = { 0.03f, 0.07f, 0.1f };
            


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
            renderer.DrawVB(cubeVA, cubeVB, pureColor);
            
            ////////////////////////////////////


            envMapShader.Bind();
            envMapShader.SetUniform1i("skybox", skyboxTexture.GetTexSlotID());
            envMapShader.SetUniform1i("normalMap", tex004.GetTexSlotID());
            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(0.0f, 0.0f, -10.0f));
            model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));
            rotation += 0.05f;
            model = glm::scale(model, glm::vec3(1.0f));
            envMapShader.SetUniformMatrix4fv("model", glm::value_ptr(model));
            envMapShader.SetUniformMatrix4fv("view", glm::value_ptr(view));
            envMapShader.SetUniformMatrix4fv("projection", glm::value_ptr(projection));
            envMapShader.SetUniform3f("viewPos", cameraPos.x, cameraPos.y, cameraPos.z);
            renderer.DrawVB(statueVA, statueVB, envMapShader);
            /*
            normalShader.Bind();
            normalShader.SetUniform1i("diffuseMap", tex001.GetTexSlotID());
            normalShader.SetUniform1i("specularTexture", tex002.GetTexSlotID());
            normalShader.SetUniform1i("normalMap", tex004.GetTexSlotID());
            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(0.0f, 0.0f, -10.0f));
            model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));
            rotation += 0.05f;
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
            renderer.DrawVB(statueVA, statueVB, normalShader);   
            */
            
            ////////////////////////////////////SKYBOX
            GLCall(glDepthFunc(GL_LEQUAL));
            skyboxShader.Bind();           
            skyboxTexture.Bind();
            skyboxShader.SetUniformMatrix4fv("view", glm::value_ptr(glm::mat4(glm::mat3(view))));
            skyboxShader.SetUniformMatrix4fv("projection", glm::value_ptr(projection));
            skyboxShader.SetUniform1i("skybox", skyboxTexture.GetTexSlotID());
            renderer.DrawVB(skyboxVA, skyboxVB, skyboxShader);
            GLCall(glDepthFunc(GL_LESS));
            /////////////////////////////////////////////////
            
                        //tone mapping
            screenFB.BindDraw();
            screenFB.Blit(frameWidth, frameHeight);
            screenFB.Unbind();
            renderer.Clear(glm::vec4(1.0f));
            toneMappingShader.Bind();
            toneMappingShader.SetUniform1i("screenTexture", screenFBTexture.GetTexSlotID());
            renderer.DrawVB(vaFB, vbFB, toneMappingShader);

            /*
            //gaussian blur
            bool horizontal = true;
            for (unsigned int i = 0; i < 5; i++)
            {
                screenFB.BindDraw();
                screenFB.Blit(frameWidth, frameHeight);
                screenFB.Unbind();
                renderer.Clear(glm::vec4(1.0f));            
                gaussianBlurShader.Bind();
                gaussianBlurShader.SetUniform1i("screenTexture", screenFBTexture.GetTexSlotID());
                gaussianBlurShader.SetUniform1i("horizontal", horizontal);
                renderer.DrawVB(vaFB, vbFB, gaussianBlurShader);
                horizontal = !horizontal;
            }
          */
            mainFB.DisableDepthTest();
            mainPostFB.BindDraw(); 
            //renderer.Clear(glm::vec4(0.0f));
            mainPostFB.Blit(frameWidth, frameHeight);                      
            //mainPostFB.Unbind();            
            
            mainFB.BindRead(); 
            gaussBlurFB.BindDraw();
            renderer.Clear(glm::vec4(0.0f));
            gaussBlurFB.Blit(frameWidth, frameHeight);            
            thresholdShader.Bind();
            thresholdShader.SetUniform1i("screenTexture", textureGaussBlurFB.GetTexSlotID());
            renderer.DrawVB(vaFB, vbFB, thresholdShader);
            
            gaussBlurFB.Bind();
            
            bool horizontal = true;
            for (unsigned int i = 0; i < 10; i++)
            {                      
                gaussianBlurShader.Bind();
                gaussianBlurShader.SetUniform1i("screenTexture", textureGaussBlurFB.GetTexSlotID());
                gaussianBlurShader.SetUniform1i("horizontal", horizontal);
                renderer.DrawVB(vaFB, vbFB, gaussianBlurShader);
                horizontal = !horizontal;
            }
            
            
            //gaussBlurFB.Unbind();
            //renderer.Clear(glm::vec4(0.0f));
            //
            //mainFB.Bind();
            //bool horizontal = true;
            //for (unsigned int i = 0; i < 10; i++)
            //{
            //    gaussBlurFB.Bind();
            //    gaussBlurFB.Unbind();
            //    renderer.Clear(glm::vec4(1.0f));
            //    gaussianBlurShader.Bind();
            //    gaussianBlurShader.SetUniform1i("screenTexture", textureGaussBlurFB.GetTexSlotID());                                       
            //    gaussianBlurShader.SetUniform1i("horizontal", true);
            //    renderer.DrawVB(vaFB, vbFB, gaussianBlurShader);
            //    horizontal = !horizontal;
            //}            
            //renderer.Clear(glm::vec4(0.0f));

            mainFB.Unbind();
            renderer.Clear(glm::vec4(0.0f));
            blendShader.Bind();
            blendShader.SetUniform1i("textureToBlend01", mainPostFBTexture.GetTexSlotID());
            blendShader.SetUniform1i("textureToBlend02", textureGaussBlurFB.GetTexSlotID());           
            renderer.DrawVB(vaFB, vbFB, blendShader);


         
            renderer.SwapBuffers();        
        }
    }
    renderer.Terminate();
}