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
const unsigned char aaSamples = renderer.GetAASamples();

void renderCube()
{
    float vertices[] = {
            // back face
            -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
             1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
             1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
             1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
            -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
            -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
            // front face
            -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
             1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
             1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
             1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
            -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
            -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
            // left face
            -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
            -1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
            -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
            -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
            -1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
            -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
            // right face
             1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
             1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
             1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
             1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
             1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
             1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
            // bottom face
            -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
             1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
             1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
             1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
            -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
            -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
            // top face
            -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
             1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
             1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
             1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
            -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
            -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left        
        };
    VertexArray cubeVA;
    VertexBuffer cubeVB(vertices, sizeof(vertices));
    VertexBufferLayout cubeLayout;
    cubeLayout.Push<float>(3); cubeLayout.Push<float>(3); cubeLayout.Push<float>(2);
    cubeVA.AddBuffer(cubeVB, cubeLayout);
    cubeVA.Bind();
    GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));
    GLCall(glBindVertexArray(0));
}

void renderQuad()
{

    float quadVertices[] = {
        // positions        // texture Coords
        -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
         1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
         1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
    };
    // setup plane VAO
    VertexArray quadVA;
    VertexBuffer quadVB(quadVertices, sizeof(quadVertices));
    VertexBufferLayout quadLayout;
    quadLayout.Push<float>(3); quadLayout.Push<float>(2);
    quadVA.AddBuffer(quadVB, quadLayout);
    quadVA.Bind();
    GLCall(glDrawArrays(GL_TRIANGLE_STRIP, 0, 4));
    GLCall(glBindVertexArray(0));
}

int main(void)
{

    if (!renderer.Init()) return -1;
    {
        //----------------------------//
        FrameBuffer mainFB;
        Texture textureMainFB(frameWidth, frameHeight, (unsigned char)aaSamples);
        textureMainFB.Bind(11);
        RenderBuffer mainRB(frameWidth, frameHeight, aaSamples);
        mainFB.CheckComplitness();
        mainFB.Unbind();

        FrameBuffer screenFB;
        Texture screenFBTexture(frameWidth, frameHeight, (unsigned char)aaSamples);
        screenFBTexture.Bind(10);
        screenFB.CheckComplitness();
        screenFB.Unbind();   

        FrameBuffer captureFB;
        int BG_Width = 2048;
        int BG_Height = 2048;        
        RenderBuffer captureRB(BG_Width, BG_Height);
        captureFB.CheckComplitness();

        //shader generation
        Shader toneMappingShader("res/shaders/ToneMapping.vert", "res/shaders/ToneMapping.frag");
        Shader normalShader("res/shaders/NormalMap.vert", "res/shaders/NormalMap.frag");
        Shader pureColor("res/shaders/Color.vert", "res/shaders/Color.frag");
        Shader gaussianBlurShader("res/shaders/GaussianBlur.vert", "res/shaders/GaussianBlur.frag");
        Shader blendShader("res/shaders/Blend.vert", "res/shaders/Blend.frag");
        Shader thresholdShader("res/shaders/Threshold.vert", "res/shaders/Threshold.frag");
        Shader skyboxShader("res/shaders/Skybox.vert", "res/shaders/Skybox.frag");
        Shader envMapShader("res/shaders/EnvironmentMap.vert", "res/shaders/EnvironmentMap.frag");

        Shader pbrShader("res/shaders/pbr.vert", "res/shaders/pbr.frag");
        pbrShader.Bind();
        pbrShader.SetUniform1i("irradianceMap", 0);
        pbrShader.SetUniform1i("prefilterMap", 1);
        pbrShader.SetUniform1i("brdfLUT", 2);
        pbrShader.SetUniform1i("albedoMap", 3);
        pbrShader.SetUniform1i("normalMap", 4);
        pbrShader.SetUniform1i("metallicMap", 5);
        pbrShader.SetUniform1i("roughnessMap", 6);
        pbrShader.SetUniform1i("aoMap", 7);
        Shader equirectangularToCubemapShader("res/shaders/cubemap.vert", "res/shaders/equirectangular_to_cubemap.frag");
        Shader irradianceShader("res/shaders/cubemap.vert", "res/shaders/irradiance_convolution.frag");
        Shader prefilterShader("res/shaders/cubemap.vert", "res/shaders/prefilter.frag");
        Shader brdfShader("res/shaders/brdf.vert", "res/shaders/brdf.frag");
        Shader backgroundShader("res/shaders/background.vert", "res/shaders/background.frag");

        backgroundShader.Bind();
        backgroundShader.SetUniform1i("environmentMap", 0);

       // glm::vec3 lightPosition = { 7.0f, 11.0f, 0.0f };
        glm::vec3 lightPosition = { 7.0f, 20.0f, -25.0f };
        glm::vec3 lightColor = { 10.0f, 10.0f, 10.0f };
        lightColor = lightColor * glm::vec3(200.0);

        Texture hdrTexture("res/textures/carb_08_XXL.hdr");
        Texture envCubemap(BG_Width, BG_Height, GL_LINEAR_MIPMAP_LINEAR);
        Texture irradianceMap(32, 32, GL_LINEAR);
        Texture prefilterMap(128, 128, GL_LINEAR_MIPMAP_LINEAR);

        /////////////////////////CUBEMAP PREPARE BEGIN///////////////////////////////////
        glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
        glm::mat4 captureViews[] =
        {
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
        };

        equirectangularToCubemapShader.Bind();
        equirectangularToCubemapShader.SetUniform1i("equirectangularMap", 0);
        equirectangularToCubemapShader.SetUniformMatrix4fv("projection", glm::value_ptr(captureProjection));

        hdrTexture.Bind(0);

        GLCall(glViewport(0, 0, BG_Width, BG_Height)); // don't forget to configure the viewport to the capture dimensions.
        captureFB.Bind();

        for (unsigned int i = 0; i < 6; ++i)
        {
            equirectangularToCubemapShader.SetUniformMatrix4fv("view", glm::value_ptr(captureViews[i]));
            GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, envCubemap.GetID(), 0));
            GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

            renderCube();
        }
        captureFB.Unbind();

        envCubemap.Bind();
        GLCall(glGenerateMipmap(GL_TEXTURE_CUBE_MAP));        

        captureFB.Bind();
        captureRB.Bind();
        GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 32, 32));

        irradianceShader.Bind();
        irradianceShader.SetUniform1i("environmentMap", 0);
        irradianceShader.SetUniformMatrix4fv("projection", glm::value_ptr(captureProjection));
        envCubemap.Bind(0);

        GLCall(glViewport(0, 0, 32, 32)); // don't forget to configure the viewport to the capture dimensions.
        captureFB.Bind();

        for (unsigned int i = 0; i < 6; ++i)
        {
            irradianceShader.SetUniformMatrix4fv("view", glm::value_ptr(captureViews[i]));
            GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, irradianceMap.GetID(), 0));
            GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
            renderCube();
        }
        captureFB.Unbind();

        
        prefilterMap.Bind();
        GLCall(glGenerateMipmap(GL_TEXTURE_CUBE_MAP));

        prefilterShader.Bind();
        prefilterShader.SetUniform1i("environmentMap", 0);
        prefilterShader.SetUniformMatrix4fv("projection", glm::value_ptr(captureProjection));
        envCubemap.Bind(0);

        captureFB.Bind();

        unsigned int maxMipLevels = 5;
        for (unsigned int mip = 0; mip < maxMipLevels; ++mip)
        {
            // reisze framebuffer according to mip-level size.
            unsigned int mipWidth = static_cast<unsigned int>(128 * std::pow(0.5, mip));
            unsigned int mipHeight = static_cast<unsigned int>(128 * std::pow(0.5, mip));
            captureRB.Bind();
            GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mipWidth, mipHeight));
            GLCall(glViewport(0, 0, mipWidth, mipHeight));

            float roughness = (float)mip / (float)(maxMipLevels - 1);
            prefilterShader.SetUniform1f("roughness", roughness);
            for (unsigned int i = 0; i < 6; ++i)
            {
                prefilterShader.SetUniformMatrix4fv("view", glm::value_ptr(captureViews[i]));
                GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, prefilterMap.GetID(), mip));

                GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
                renderCube();
            }
        }
        captureFB.Unbind();

        unsigned int brdfLUTTexture;
        GLCall(glGenTextures(1, &brdfLUTTexture));

        // pre-allocate enough memory for the LUT texture.
        GLCall(glBindTexture(GL_TEXTURE_2D, brdfLUTTexture));
        GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, BG_Width, BG_Height, 0, GL_RG, GL_FLOAT, 0));
        // be sure to set wrapping mode to GL_CLAMP_TO_EDGE
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

        // then re-configure capture framebuffer object and render screen-space quad with BRDF shader.
        captureFB.Bind();
        captureRB.Bind();
        GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, BG_Width, BG_Height));
        GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, brdfLUTTexture, 0));

        GLCall(glViewport(0, 0, BG_Width, BG_Height));
        brdfShader.Bind();
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
        renderQuad();

        captureFB.Unbind();
        /////////////////////////CUBEMAP PREPARE END///////////////////////////////////


        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(renderer.GetFov()), (float)frameWidth / (float)frameHeight, 0.1f, 100.0f);

        pbrShader.Bind();
        pbrShader.SetUniformMatrix4fv("projection", glm::value_ptr(projection));
        backgroundShader.Bind();
        backgroundShader.SetUniformMatrix4fv("projection", glm::value_ptr(projection));
        
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
        
        ObjImporter objImport2("res/models/cube.obj", false);
        VertexArray cubeVA;
        VertexBuffer cubeVB(objImport2.GetVertecies(), objImport2.GetVertCount());
        VertexBufferLayout cubeLayout;
        cubeLayout.Push<float>(3); cubeLayout.Push<float>(3); cubeLayout.Push<float>(2); cubeLayout.Push<float>(3);
        cubeVA.AddBuffer(cubeVB, cubeLayout);
        
        ObjImporter objImport3("res/models/statue_hard.obj", true);
        VertexArray statueVA;
        VertexBuffer statueVB(objImport3.GetVertecies(), objImport3.GetVertCount());
        VertexBufferLayout layoutTBN;
        layoutTBN.Push<float>(3); layoutTBN.Push<float>(3); layoutTBN.Push<float>(2); layoutTBN.Push<float>(3); layoutTBN.Push<float>(3); layoutTBN.Push<float>(3);
        statueVA.AddBuffer(statueVB, layoutTBN);

        float skyboxVertices[] = {
            // back face
            -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
             1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
             1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
             1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
            -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
            -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
            // front face
            -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
             1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
             1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
             1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
            -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
            -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
            // left face
            -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
            -1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
            -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
            -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
            -1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
            -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
            // right face
             1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
             1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
             1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
             1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
             1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
             1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
            // bottom face
            -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
             1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
             1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
             1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
            -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
            -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
            // top face
            -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
             1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
             1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
             1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
            -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
            -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left        
        };
        VertexArray skyboxVA;
        VertexBuffer skyboxVB(skyboxVertices, sizeof(skyboxVertices));
        VertexBufferLayout skyboxLayout;
        skyboxLayout.Push<float>(3); skyboxLayout.Push<float>(3); skyboxLayout.Push<float>(2);
        skyboxVA.AddBuffer(skyboxVB, skyboxLayout);

        //texture generation  
        Texture albedo("res/textures/tgziabifa_4K_Albedo.jpg", true);
        Texture normal("res/textures/tgziabifa_4K_Normal_LOD0.jpg", false);
        //Texture metallic("res/textures/tgziabifa_4K_Displacement.jpg", true);
        Texture roughness("res/textures/tgziabifa_4K_Specular.jpg", true);
        Texture ao("res/textures/tgziabifa_4K_Cavity.jpg", false);

        GLCall(glViewport(0, 0, frameWidth, frameHeight));

        float rotation = 0.0f;
        glm::vec4 environmentColor = glm::vec4(0.5f, 1.0f, 3.0f, 1.0f);
        glm::vec3 cameraPos = glm::vec3();
        glm::vec3 cameraFront = glm::vec3();
        glm::vec3 cameraUp = glm::vec3();
        //MAIN LOOP
        while (renderer.IsWindowClosed())
        {
            renderer.ProceedInput();
            cameraPos = renderer.GetCameraPos();
            cameraFront = renderer.GetCameraFront();
            cameraUp = renderer.GetCameraUp();

            //mainFB.Bind();  
            renderer.Clear(environmentColor);
           // mainFB.EnableDepthTest();

            irradianceMap.Bind(0);
            prefilterMap.Bind(1);
            GLCall(glActiveTexture(GL_TEXTURE2));
            GLCall(glBindTexture(GL_TEXTURE_2D, brdfLUTTexture));

            /* Render here */
            pbrShader.Bind();
            model = glm::mat4(1.0f);
            view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);   
            pbrShader.SetUniformMatrix4fv("view", glm::value_ptr(view));
            pbrShader.SetUniform3f("camPos", cameraPos.x, cameraPos.y, cameraPos.z);           
            albedo.Bind(3);
            normal.Bind(4);
            //metallic.Bind(5);
            roughness.Bind(6);
            ao.Bind(7);
            pbrShader.SetUniform1i("albedoMap", albedo.GetTexSlotID());
            pbrShader.SetUniform1i("roughnessMap", roughness.GetTexSlotID());
            pbrShader.SetUniform1i("normalMap", normal.GetTexSlotID());
            pbrShader.SetUniform1i("aoMap", ao.GetTexSlotID());
            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(0.0f, 0.0f, -10.0f));
            model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));
            rotation += 0.05f;
            model = glm::scale(model, glm::vec3(1.0f));
            pbrShader.SetUniformMatrix4fv("projection", glm::value_ptr(projection));
            pbrShader.SetUniformMatrix4fv("model", glm::value_ptr(model));           
            pbrShader.SetUniform3f("lightPositions[0]", lightPosition.x, lightPosition.y, lightPosition.z);
            pbrShader.SetUniform3f("lightColors[0]", lightColor.x, lightColor.y, lightColor.z);
            renderer.DrawVB(statueVA, statueVB, pbrShader);

            //////////////////////////////////LIGHT_SOURCE
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
            
            ////////////////////////////////////SKYBOX
            //GLCall(glDepthFunc(GL_LEQUAL));
            backgroundShader.Bind();
            backgroundShader.SetUniformMatrix4fv("view", glm::value_ptr(view));
            envCubemap.Bind(0);
            renderCube();
            //skyboxShader.Bind();
            //skyboxTexture.Bind();
            //skyboxShader.SetUniformMatrix4fv("view", glm::value_ptr(glm::mat4(glm::mat3(view))));
            //skyboxShader.SetUniformMatrix4fv("projection", glm::value_ptr(projection));
            //skyboxShader.SetUniform1i("skybox", skyboxTexture.GetTexSlotID());
            //renderer.DrawVB(skyboxVA, skyboxVB, skyboxShader);
            //GLCall(glDepthFunc(GL_LESS));
            /////////////////////////////////////////////////

            ////tone mapping           
            //screenFB.BindDraw();
            //screenFB.Blit(frameWidth, frameHeight);
            //screenFB.Unbind();
            //renderer.Clear(glm::vec4(1.0f));
            //toneMappingShader.Bind();         
            //toneMappingShader.SetUniform1i("screenTexture", screenFBTexture.GetTexSlotID());
            //renderer.DrawVB(vaFB, vbFB, toneMappingShader);

            renderer.SwapBuffers();
        }
    }
    renderer.Terminate();
}

