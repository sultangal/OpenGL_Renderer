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
#include "File_Texture.h"
#include "IBL_Texture.h"
#include "Bloom_Texture.h"
#include "FrameBuffer.h"
#include "FrameBufferTexture.h"
#include "RenderBuffer.h"
#include "PerformanceMeasure.h"

Renderer renderer;
int m_FrameWidth = renderer.GetWidth();
int m_FrameHeight = renderer.GetHeight();
const unsigned char aaSamples = renderer.GetAASamples();
int BG_Width = 2048;
int BG_Height = 2048;

void inline RenderCube()
{
    GLCall(glCullFace(GL_FRONT));
    float vertices[] = {
            // back face
            -1.0f, -1.0f, -1.0f, // bottom-left
             1.0f,  1.0f, -1.0f, // top-right
             1.0f, -1.0f, -1.0f, // bottom-right         
             1.0f,  1.0f, -1.0f, // top-right
            -1.0f, -1.0f, -1.0f, // bottom-left
            -1.0f,  1.0f, -1.0f, // top-left
            // front face
            -1.0f, -1.0f,  1.0f, // bottom-left
             1.0f, -1.0f,  1.0f, // bottom-right
             1.0f,  1.0f,  1.0f, // top-right
             1.0f,  1.0f,  1.0f, // top-right
            -1.0f,  1.0f,  1.0f, // top-left
            -1.0f, -1.0f,  1.0f, // bottom-left
            // left face
            -1.0f,  1.0f,  1.0f, // top-right
            -1.0f,  1.0f, -1.0f, // top-left
            -1.0f, -1.0f, -1.0f, // bottom-left
            -1.0f, -1.0f, -1.0f, // bottom-left
            -1.0f, -1.0f,  1.0f, // bottom-right
            -1.0f,  1.0f,  1.0f, // top-right
            // right face
             1.0f,  1.0f,  1.0f, // top-left
             1.0f, -1.0f, -1.0f, // bottom-right
             1.0f,  1.0f, -1.0f, // top-right         
             1.0f, -1.0f, -1.0f, // bottom-right
             1.0f,  1.0f,  1.0f, // top-left
             1.0f, -1.0f,  1.0f, // bottom-left     
            // bottom face
            -1.0f, -1.0f, -1.0f, // top-right
             1.0f, -1.0f, -1.0f, // top-left
             1.0f, -1.0f,  1.0f, // bottom-left
             1.0f, -1.0f,  1.0f, // bottom-left
            -1.0f, -1.0f,  1.0f, // bottom-right
            -1.0f, -1.0f, -1.0f, // top-right
            // top face
            -1.0f,  1.0f, -1.0f, // top-left
             1.0f,  1.0f , 1.0f, // bottom-right
             1.0f,  1.0f, -1.0f, // top-right     
             1.0f,  1.0f,  1.0f, // bottom-right
            -1.0f,  1.0f, -1.0f, // top-left
            -1.0f,  1.0f,  1.0f  // bottom-left        
        };
    VertexArray cubeVA;
    VertexBuffer cubeVB(vertices, sizeof(vertices));
    VertexBufferLayout cubeLayout;
    cubeLayout.Push<float>(3);
    cubeVA.AddBuffer(cubeVB, cubeLayout);
    renderer.DrawVB(cubeVA, cubeVB);
    GLCall(glCullFace(GL_BACK));
}

void inline RenderQuad()
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
    renderer.DrawVB_Strip(quadVA, quadVB);
}

void inline PrepareIBL(File_Texture& hdrTEX, IBL_Texture& envCubeMAP, IBL_Texture& irradianceMAP, IBL_Texture& prefilterMAP, IBL_Texture& brdfLutMAP, FrameBuffer& captureFB, RenderBuffer& captureRB)
{

    Shader equirectangularToCubemapShader("res/shaders/cubemap.vert", "res/shaders/equirectangular_to_cubemap.frag");
    Shader irradianceShader("res/shaders/cubemap.vert", "res/shaders/irradiance_convolution.frag");
    Shader prefilterShader("res/shaders/cubemap.vert", "res/shaders/prefilter.frag");
    Shader brdfShader("res/shaders/brdf.vert", "res/shaders/brdf.frag");
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

    hdrTEX.Bind(0);

    renderer.ConfigureViewport(BG_Width, BG_Height); // don't forget to configure the viewport to the capture dimensions.        
    captureFB.Bind();

    for (unsigned int i = 0; i < 6; ++i)
    {
        equirectangularToCubemapShader.SetUniformMatrix4fv("view", glm::value_ptr(captureViews[i]));
        envCubeMAP.AttachCubeTexToCurrFB(i, 0);
        renderer.Clear();
        RenderCube();
    }

    envCubeMAP.Bind();
    envCubeMAP.GenMipMap();

    captureRB.Bind();
    captureRB.ConfigureRBStorage(32, 32);

    irradianceShader.Bind();
    irradianceShader.SetUniform1i("environmentMap", 0);
    irradianceShader.SetUniformMatrix4fv("projection", glm::value_ptr(captureProjection));
    envCubeMAP.Bind(0);

    renderer.ConfigureViewport(32, 32); // don't forget to configure the viewport to the capture dimensions.       

    for (unsigned int i = 0; i < 6; ++i)
    {
        irradianceShader.SetUniformMatrix4fv("view", glm::value_ptr(captureViews[i]));
        irradianceMAP.AttachCubeTexToCurrFB(i, 0);
        renderer.Clear();
        RenderCube();
    }

    prefilterMAP.Bind();
    prefilterMAP.GenMipMap();

    prefilterShader.Bind();
    prefilterShader.SetUniform1i("environmentMap", 0);
    prefilterShader.SetUniformMatrix4fv("projection", glm::value_ptr(captureProjection));
    envCubeMAP.Bind(0);

    unsigned int maxMipLevels = 5;
    for (unsigned int mip = 0; mip < maxMipLevels; ++mip)
    {
        // reisze framebuffer according to mip-level size.
        unsigned int mipWidth = static_cast<unsigned int>(128 * std::pow(0.5, mip));
        unsigned int mipHeight = static_cast<unsigned int>(128 * std::pow(0.5, mip));
        captureRB.Bind();
        captureRB.ConfigureRBStorage(mipWidth, mipHeight);
        renderer.ConfigureViewport(mipWidth, mipHeight);

        float roughness = (float)mip / (float)(maxMipLevels - 1);
        prefilterShader.SetUniform1f("roughness", roughness);

        for (unsigned int i = 0; i < 6; ++i)
        {
            prefilterShader.SetUniformMatrix4fv("view", glm::value_ptr(captureViews[i]));
            prefilterMAP.AttachCubeTexToCurrFB(i, mip);
            renderer.Clear();
            RenderCube();
        }
    }

    captureRB.Bind();
    captureRB.ConfigureRBStorage(BG_Width, BG_Height);
    brdfLutMAP.AttachTexToCurrFB();

    renderer.ConfigureViewport(BG_Width, BG_Height);
    brdfShader.Bind();
    renderer.Clear();
    RenderQuad();
    captureFB.Unbind();
    /////////////////////////CUBEMAP PREPARE END///////////////////////////////////

}

int main(void)
{
    if (!renderer.Init()) return -1;
    {
        PerformanceMeasure measure("Starting time");

        FrameBuffer mainFB;
        FrameBufferTexture mainFBMap(m_FrameWidth, m_FrameHeight, (unsigned char)aaSamples);
        mainFBMap.Bind(11);
        RenderBuffer mainRB(m_FrameWidth, m_FrameHeight, aaSamples);
        mainFB.CheckComplitness();
        mainFB.Unbind();

        FrameBuffer screenFB;
        FrameBufferTexture screenFBMap(m_FrameWidth, m_FrameHeight);
        screenFBMap.Bind(10);
        screenFB.CheckComplitness();
        screenFB.Unbind();  

        FrameBuffer blurredFB;
        std::vector<BloomMip> mipIDs;
        Bloom_Texture blurredFBMap;
        blurredFBMap.GetMipChainTexIDs(mipIDs);
        blurredFB.CheckComplitness();
        blurredFB.Unbind();

        FrameBuffer beautyFB;       
        FrameBufferTexture beautyFBMap(m_FrameWidth, m_FrameHeight);
        beautyFBMap.Bind(8);
        beautyFB.CheckComplitness();
        beautyFB.Unbind();

        //--IBL preparation--//
        FrameBuffer captureFB;
        RenderBuffer captureRB(BG_Width, BG_Height);
        captureFB.CheckComplitness();        
        File_Texture hdrTEX("res/textures/carb_05_XXL.hdr");
        IBL_Texture envCubeMAP(BG_Width, BG_Height, true);
        IBL_Texture irradianceMAP(32, 32, false);
        IBL_Texture prefilterMAP(128, 128, true);
        IBL_Texture brdfLutMAP(BG_Width, BG_Height);
        PrepareIBL(hdrTEX, envCubeMAP, irradianceMAP, prefilterMAP, brdfLutMAP, captureFB, captureRB);
        //----------------------------//

        //--Shader generation--//
        Shader compositingShader("res/shaders/ScreenQuad.vert", "res/shaders/Compositing.frag");
        Shader normalShader("res/shaders/NormalMap.vert", "res/shaders/NormalMap.frag");
        Shader pureColor("res/shaders/Color.vert", "res/shaders/Color.frag");
        Shader gaussianBlurShader("res/shaders/ScreenQuad.vert", "res/shaders/GaussianBlur.frag");
        Shader blendShader("res/shaders/ScreenQuad.vert", "res/shaders/Blend.frag");
        Shader thresholdShader("res/shaders/ScreenQuad.vert", "res/shaders/Threshold.frag");
        Shader skyboxShader("res/shaders/Skybox.vert", "res/shaders/Skybox.frag");
        Shader envMapShader("res/shaders/EnvironmentMap.vert", "res/shaders/EnvironmentMap.frag");
        Shader exposureShader("res/shaders/ScreenQuad.vert", "res/shaders/Exposure.frag");
        Shader downsamplingShader("res/shaders/ScreenQuad.vert", "res/shaders/Downsampling.frag");
        Shader upsamplingShader("res/shaders/ScreenQuad.vert", "res/shaders/Upsampling.frag");

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

        Shader backgroundShader("res/shaders/background.vert", "res/shaders/background.frag");
        backgroundShader.Bind();
        backgroundShader.SetUniform1i("environmentMap", 0);
        //----------------------------//

        //glm::vec3 lightPosition = { 7.0f, 11.0f, 0.0f };
        glm::vec3 lightPosition = { 7.0f, 20.0f, -25.0f };
        glm::vec3 lightColor = { 100.0f, 20.0f, 1.0f };
        //glm::vec3 lightColor = { 100.0f, 39.0f, 10.0f };
        lightColor = lightColor * glm::vec3(30.0);

        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(renderer.GetFov()), (float)m_FrameWidth / (float)m_FrameHeight, 0.1f, 100.0f);
        
        pbrShader.Bind();
        pbrShader.SetUniformMatrix4fv("projection", glm::value_ptr(projection));
        backgroundShader.Bind();
        backgroundShader.SetUniformMatrix4fv("projection", glm::value_ptr(projection));
        
        //--Screen quad--//
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
        
        //--Models import and vertex buffer, vertex array, layout generation for that models--//         
        ObjImporter objCube("res/models/geosphere.obj", false);
        VertexArray cubeVA;
        VertexBuffer cubeVB(objCube.GetVertecies(), objCube.GetVertCount());
        VertexBufferLayout cubeLayout;
        cubeLayout.Push<float>(3); cubeLayout.Push<float>(2); cubeLayout.Push<float>(3);
        cubeVA.AddBuffer(cubeVB, cubeLayout);
        
        ObjImporter objStatue("res/models/statue_hard.obj", true);
        VertexArray statueVA;
        VertexBuffer statueVB(objStatue.GetVertecies(), objStatue.GetVertCount());
        VertexBufferLayout statueLayout;
        statueLayout.Push<float>(3);  statueLayout.Push<float>(2); statueLayout.Push<float>(3); statueLayout.Push<float>(3); statueLayout.Push<float>(3);
        statueVA.AddBuffer(statueVB, statueLayout);
        //----------------------------//

        //--Texture generation--//  
        //Texture albedoTEX("res/textures/tgziabifa_4K_Albedo.jpg", true);
        File_Texture normalTEX("res/textures/tgziabifa_4K_Normal_LOD0.jpg", false);
        //Texture metallic("res/textures/tgziabifa_4K_Displacement.jpg", true);
        File_Texture roughnessTEX("res/textures/tgziabifa_4K_Roughness.jpg", true);
        File_Texture aoTEX("res/textures/tgziabifa_4K_Cavity.jpg", false);
        //----------------------------//

        float rotation = 0.0f;
        glm::vec4 environmentColor = glm::vec4(0.5f, 1.0f, 3.0f, 1.0f);
        glm::vec3 cameraPos = glm::vec3();
        glm::vec3 cameraFront = glm::vec3();
        glm::vec3 cameraUp = glm::vec3();
        renderer.ConfigureViewport(m_FrameWidth, m_FrameHeight);

        measure.Stop();
        //MAIN LOOP//
        while (renderer.IsWindowClosed())
        {           
            renderer.ProceedInput();
            cameraPos = renderer.GetCameraPos();
            cameraFront = renderer.GetCameraFront();
            cameraUp = renderer.GetCameraUp();
            view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
            
            mainFB.Bind();                       
            renderer.Clear(environmentColor);
            mainFB.EnableDepthTest();

            irradianceMAP.Bind(0);
            prefilterMAP.Bind(1);
            brdfLutMAP.Bind(2);

            //* RENDER HERE */
            pbrShader.Bind();         
            pbrShader.SetUniformMatrix4fv("view", glm::value_ptr(view));
            pbrShader.SetUniform3f("camPos", cameraPos.x, cameraPos.y, cameraPos.z);           
            //albedoTEX.Bind(3);
            normalTEX.Bind(4);
            //metallic.Bind(5);
            roughnessTEX.Bind(6);
            aoTEX.Bind(7);
            //pbrShader.SetUniform1i("albedoMap", albedoTEX.GetTexSlotID());
            pbrShader.SetUniform1i("roughnessMap", roughnessTEX.GetTexSlotID());
            pbrShader.SetUniform1i("normalMap", normalTEX.GetTexSlotID());
            pbrShader.SetUniform1i("aoMap", aoTEX.GetTexSlotID());
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
            //----------------------------//
            
            //--Light source--//
            pureColor.Bind();
            pureColor.SetUniformMatrix4fv("view", glm::value_ptr(view));
            pureColor.SetUniformMatrix4fv("projection", glm::value_ptr(projection));
            pureColor.SetUniform3f("lightColor", lightColor.x, lightColor.y, lightColor.z);
            model = glm::mat4(1.0f);
            model = glm::translate(model, { lightPosition.x, lightPosition.y, lightPosition.z });
            model = glm::scale(model, glm::vec3(0.5f));
            pureColor.SetUniformMatrix4fv("model", glm::value_ptr(model));
            renderer.DrawVB(cubeVA, cubeVB, pureColor);
            //----------------------------//
           
            //--Skybox--//
            GLCall(glDepthFunc(GL_LEQUAL));
            backgroundShader.Bind();
            backgroundShader.SetUniformMatrix4fv("view", glm::value_ptr(view));
            envCubeMAP.Bind(0);
            RenderCube();
            //skyboxShader.Bind();
            //skyboxTexture.Bind();
            //skyboxShader.SetUniformMatrix4fv("view", glm::value_ptr(glm::mat4(glm::mat3(view))));
            //skyboxShader.SetUniformMatrix4fv("projection", glm::value_ptr(projection));
            //skyboxShader.SetUniform1i("skybox", skyboxTexture.GetTexSlotID());
            //renderer.DrawVB(skyboxVA, skyboxVB, skyboxShader);
            GLCall(glDepthFunc(GL_LESS));
            //----------------------------//

            //POST-PROCESSING//
            screenFB.BindDraw();
            screenFB.Blit(m_FrameWidth, m_FrameHeight);
            beautyFB.Bind();
            exposureShader.Bind();
            exposureShader.SetUniform1i("screenTexture", screenFBMap.GetTexSlotID());
            exposureShader.SetUniform1f("exposure", 2.5);
            renderer.DrawVB(vaFB, vbFB, exposureShader);

            screenFB.BindDraw();
            screenFB.Blit(m_FrameWidth, m_FrameHeight);
            screenFB.Bind();
            exposureShader.Bind();
            exposureShader.SetUniform1i("screenTexture", screenFBMap.GetTexSlotID());
            exposureShader.SetUniform1f("exposure", 0.006);
            renderer.DrawVB(vaFB, vbFB, exposureShader);

            //--Glow--// 
            blurredFB.Bind();
            //Downscale           
            blurredFBMap.Bind(9, mipIDs[0].mipId);
            renderer.ConfigureViewport(m_FrameWidth, m_FrameHeight);
            blurredFBMap.AttachTexToCurrFB(mipIDs[0].mipId);
            downsamplingShader.Bind();
            downsamplingShader.SetUniform1f("srcWidth", (float)mipIDs[0].mipWidth);
            downsamplingShader.SetUniform1f("srcHeight", (float)mipIDs[0].mipHeight);
            downsamplingShader.SetUniform1i("srcTexture", screenFBMap.GetTexSlotID());
            renderer.DrawVB(vaFB, vbFB, downsamplingShader);

            for (unsigned int mipLevel = 1; mipLevel < mipIDs.size(); mipLevel++)
            { 
                blurredFBMap.Bind(0, mipIDs[mipLevel - 1].mipId);
                blurredFBMap.Bind(9, mipIDs[mipLevel].mipId);

                renderer.ConfigureViewport(mipIDs[mipLevel].mipWidth, mipIDs[mipLevel].mipHeight);
                blurredFBMap.AttachTexToCurrFB(mipIDs[mipLevel].mipId);
                downsamplingShader.Bind();
                downsamplingShader.SetUniform1f("srcWidth", (float)mipIDs[mipLevel].mipWidth);
                downsamplingShader.SetUniform1f("srcHeight", (float)mipIDs[mipLevel].mipHeight);
                downsamplingShader.SetUniform1i("srcTexture", 0);
                renderer.DrawVB(vaFB, vbFB, downsamplingShader);
            }

            //Upscale
            for (unsigned int mipLevel = mipIDs.size()-1; mipLevel > 0; mipLevel--)
            {
                blurredFBMap.Bind(0, mipIDs[mipLevel].mipId);
                blurredFBMap.Bind(9, mipIDs[mipLevel - 1].mipId);
            
                renderer.ConfigureViewport(mipIDs[mipLevel - 1].mipWidth, mipIDs[mipLevel - 1].mipHeight);
            
                blurredFBMap.AttachTexToCurrFB(mipIDs[mipLevel-1].mipId);
                upsamplingShader.Bind();
                upsamplingShader.SetUniform1i("srcTexture", 0);
                upsamplingShader.SetUniform1i("currTexture", 9);
                renderer.DrawVB(vaFB, vbFB, upsamplingShader);
            }
            blurredFBMap.Bind(9, mipIDs[1].mipId);
            //----------------------------//

            //--Compositing--//
            blurredFB.Unbind();
            blurredFB.DisableDepthTest();
            renderer.ConfigureViewport(m_FrameWidth, m_FrameHeight);
            compositingShader.Bind();
            compositingShader.SetUniform1i("screenTexture01", blurredFBMap.GetTexSlotID());
            compositingShader.SetUniform1i("screenTexture02", beautyFBMap.GetTexSlotID());
            renderer.DrawVB(vaFB, vbFB, compositingShader);
            //----------------------------//

            renderer.SwapBuffers();
        }
    }
    renderer.Terminate();
}

