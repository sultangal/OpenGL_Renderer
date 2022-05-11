#include "pch.h"
#include "Texture.h"
#include <GL/glew.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "ErrorCheck.h"

Texture::Texture(const unsigned int& frameWidth, const unsigned int& frameHeight, int gl_tex_min_filter)
    :m_RendererID(0), m_TextureSlot(0), m_TexType(GL_TEXTURE_CUBE_MAP)
{
    GLCall(glGenTextures(1, &m_RendererID));
    GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID));
    for (unsigned int i = 0; i < 6; ++i)
    {
        GLCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, frameWidth, frameHeight, 0, GL_RGB, GL_FLOAT, nullptr));
    }
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, gl_tex_min_filter)); // enable pre-filter mipmap sampling (combatting visible dots artifact)
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
}

Texture::Texture(const std::string& filePath, bool gammaCorrected)
    :m_TexType(GL_TEXTURE_2D), m_TextureSlot(0)
{
    stbi_set_flip_vertically_on_load(true);
    int width, height, nrChannels;
    unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &nrChannels, 0);
    GLCall(glGenTextures(1, &m_RendererID));
    //AssignTexSlot(m_TextureSlot);
    if (data) {
        GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
        //GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
        if (gammaCorrected) {
            GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data));
        }
        else {
            GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data));
        }
        GLCall(glGenerateMipmap(GL_TEXTURE_2D));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
        std::cout << "[Texture][MESSAGE]::Texture : " << filePath << " - created." << std::endl;
    }
    else {
        std::cout << "[Texture][ERROR]::Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
}

Texture::Texture(const unsigned int& frameWidth, const unsigned int& frameHeight, const unsigned char& aaSamples)
    :m_TexType(GL_TEXTURE_2D_MULTISAMPLE), m_TextureSlot(0)
{
   // AssignTexSlot(m_TextureSlot);
    GLCall(glGenTextures(1, &m_RendererID));
    GLCall(glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_RendererID));
    GLCall(glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, aaSamples, GL_RGB32F, frameWidth, frameHeight, GL_TRUE));
    GLCall(glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0));
    GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, m_RendererID, 0));

    
}

Texture::Texture(const unsigned int& frameWidth, const unsigned int& frameHeight)
    :m_TexType(GL_TEXTURE_2D), m_TextureSlot(0)
{
    //AssignTexSlot(m_TextureSlot);
    GLCall(glGenTextures(1, &m_RendererID));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, frameWidth, frameHeight, 0, GL_RGB, GL_FLOAT, NULL));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_RendererID, 0));
    
}

Texture::Texture(std::string filePath)
    :m_RendererID(0), m_TextureSlot(0), m_TexType(GL_TEXTURE_2D)
{        
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    float* data = stbi_loadf(filePath.c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        GLCall(glGenTextures(1, &m_RendererID));
        //AssignTexSlot(m_TextureSlot);
        GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
        GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 0, GL_RGB, GL_FLOAT, data));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
       
        stbi_image_free(data);
        std::cout << "[Texture][MESSAGE]::Texture : " << filePath << " - created." << std::endl;
    }
    else
    {
        std::cout << "[Texture][ERROR]::Failed to load texture at path: " << filePath << std::endl;
        stbi_image_free(data);
    }   
}



void Texture::Bind(unsigned char textureSlot)
{
    m_TextureSlot = textureSlot;
    AssignTexSlot(textureSlot);
    //std::cout << "[Texture][MESSAGE]::Assigned texture slot to: " << (int)textureSlot << std::endl;
    GLCall(glBindTexture(m_TexType, m_RendererID));
}


void Texture::Bind()
{
    AssignTexSlot(m_TextureSlot);
    GLCall(glBindTexture(m_TexType, m_RendererID));
}

void Texture::Unbind()
{
    GLCall(glBindTexture(m_TexType, 0));
}

int Texture::GetID()
{
    return m_RendererID;
}
unsigned char Texture::GetTexSlotID()
{
    return m_TextureSlot;
}

void Texture::AssignTexSlot(unsigned char textureSlot) {
    if (textureSlot > 11) std::cout << "[Texture][WARNNIG]::glActiveTexture don't have so many slots! Assigning default texture slot." << std::endl;
    switch (textureSlot)
    {
    case 0:
        GLCall(glActiveTexture(GL_TEXTURE0));
        break;
    case 1:
        GLCall(glActiveTexture(GL_TEXTURE1));
        break;
    case 2:
        GLCall(glActiveTexture(GL_TEXTURE2));
        break;
    case 3:
        GLCall(glActiveTexture(GL_TEXTURE3));
        break;
    case 4:
        GLCall(glActiveTexture(GL_TEXTURE4));
        break;
    case 5:
        GLCall(glActiveTexture(GL_TEXTURE5));
        break;
    case 6:
        GLCall(glActiveTexture(GL_TEXTURE6));
        break;
    case 7:
        GLCall(glActiveTexture(GL_TEXTURE7));
        break;
    case 8:
        GLCall(glActiveTexture(GL_TEXTURE8));
        break;
    case 9:
        GLCall(glActiveTexture(GL_TEXTURE9));
        break;
    case 10:
        GLCall(glActiveTexture(GL_TEXTURE10));
        break;
    case 11:
        GLCall(glActiveTexture(GL_TEXTURE11));
        break;
    default:
        GLCall(glActiveTexture(GL_TEXTURE0));
        break;
    }
}

Texture::~Texture()
{
    glDeleteTextures(1, &m_RendererID);
}
