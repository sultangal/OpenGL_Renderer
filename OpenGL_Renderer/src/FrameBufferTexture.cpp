#include "pch.h"
#include "FrameBufferTexture.h"
#include <GL/glew.h>
#include "ErrorCheck.h"

FrameBufferTexture::FrameBufferTexture()
{
}

FrameBufferTexture::FrameBufferTexture(const unsigned int& frameWidth, const unsigned int& frameHeight, const unsigned char& aaSamples)
    :m_TexType(GL_TEXTURE_2D_MULTISAMPLE), m_TextureSlot(0)
{
    GLCall(glGenTextures(1, &m_TextureID));
    AssignTexSlot(m_TextureSlot);
    GLCall(glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_TextureID));
    GLCall(glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, aaSamples, GL_RGB32F, frameWidth, frameHeight, GL_TRUE));
    GLCall(glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0));
    GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, m_TextureID, 0));    
}

FrameBufferTexture::FrameBufferTexture(const unsigned int& frameWidth, const unsigned int& frameHeight)
    :m_TexType(GL_TEXTURE_2D), m_TextureSlot(0)
{
    GLCall(glGenTextures(1, &m_TextureID));
    AssignTexSlot(m_TextureSlot);
    GLCall(glBindTexture(GL_TEXTURE_2D, m_TextureID));
    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, frameWidth, frameHeight, 0, GL_RGB, GL_FLOAT, NULL));
    //GLCall(glGenerateMipmap(GL_TEXTURE_2D));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 13));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_TextureID, 0));
}

void FrameBufferTexture::Bind(unsigned char textureSlot)
{
    m_TextureSlot = textureSlot;
    AssignTexSlot(textureSlot);
    //std::cout << "[Texture][MESSAGE]::Assigned texture slot to: " << (int)textureSlot << std::endl;
    GLCall(glBindTexture(m_TexType, m_TextureID));
}

void FrameBufferTexture::Bind()
{
    AssignTexSlot(m_TextureSlot);
    GLCall(glBindTexture(m_TexType, m_TextureID));
}

void FrameBufferTexture::Unbind()
{
    GLCall(glBindTexture(m_TexType, 0));
}

int FrameBufferTexture::GetID()
{
    return m_TextureID;
}

unsigned char FrameBufferTexture::GetTexSlotID()
{
    return m_TextureSlot;
}

void FrameBufferTexture::AssignTexSlot(unsigned char textureSlot) {
    if (textureSlot > 11) std::cout << yellow << "[Texture][WARNNIG]::glActiveTexture don't have so many slots! Assigning default texture slot." << white << std::endl;
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

void FrameBufferTexture::AttachTexToCurrFB()
{
    GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_TexType, m_TextureID, 0));
}

void FrameBufferTexture::AttachTexToCurrFB(unsigned int mipmapLevel)
{
    GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_TexType, m_TextureID, mipmapLevel));
}

void FrameBufferTexture::AttachCubeTexToCurrFB(unsigned int i, unsigned int mipmapLevel)
{
    GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X+i, m_TextureID, mipmapLevel));
}

void FrameBufferTexture::GenMipMap()
{
    GLCall(glGenerateMipmap(m_TexType));
}

FrameBufferTexture::~FrameBufferTexture()
{
    GLCall(glDeleteTextures(1, &m_TextureID));
}
