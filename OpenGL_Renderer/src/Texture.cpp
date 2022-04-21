#include "pch.h"
#include "Texture.h"
#include <GL/glew.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "ErrorCheck.h"

Texture::Texture(const std::string& filePath, unsigned char textureSlot, bool gammaCorrected)
    :m_Data(nullptr), m_TexWidth(0), m_TexHeight(0), m_TextureSlot(textureSlot)
{
    stbi_set_flip_vertically_on_load(true);
    int nrChannels;
    m_Data = stbi_load(filePath.c_str(), &m_TexWidth, &m_TexHeight, &nrChannels, 0);
    GLCall(glGenTextures(1, &m_RendererID));

    AssignTexSlot(textureSlot);

    if (m_Data) {
        GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
        //GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
        if (gammaCorrected) {
            GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB, m_TexWidth, m_TexHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, m_Data));
        }
        else {
            GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_TexWidth, m_TexHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, m_Data));
        }

        GLCall(glGenerateMipmap(GL_TEXTURE_2D));

        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
    }
    else {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(m_Data);
}

Texture::Texture(const unsigned int& frameWidth, const unsigned int& frameHeight, const unsigned char& aaSamples, unsigned char textureSlot)
    :m_Data(nullptr), m_TexWidth(0), m_TexHeight(0), m_TextureSlot(textureSlot)
{
    AssignTexSlot(textureSlot);
    GLCall(glGenTextures(1, &m_RendererID));
    GLCall(glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_RendererID));
    GLCall(glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, aaSamples, GL_RGB16F, frameWidth, frameHeight, GL_TRUE));
    GLCall(glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0));
    GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, m_RendererID, 0));
    
}

Texture::Texture(const unsigned int& frameWidth, const unsigned int& frameHeight, unsigned char textureSlot)
    :m_Data(nullptr), m_TexWidth(0), m_TexHeight(0), m_TextureSlot(textureSlot)
{
    AssignTexSlot(textureSlot);
    GLCall(glGenTextures(1, &m_RendererID));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, frameWidth, frameHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_RendererID, 0));
    
}

void Texture::Bind(unsigned char textureSlot)
{
    m_TextureSlot = textureSlot;
    AssignTexSlot(textureSlot);
    GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
}

void Texture::Bind()
{
    AssignTexSlot(m_TextureSlot);
    GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
}

void Texture::Unbind()
{
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

unsigned char Texture::GetTexSlotID()
{
    return m_TextureSlot;
}

void Texture::AssignTexSlot(unsigned char textureSlot) {
    if (textureSlot > 11) std::cout << "[WARNNIG] glActiveTexture don't have so many slots! Assigning default texture slot." << std::endl;
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
    GLCall(glDeleteTextures(1, &m_RendererID));
}
