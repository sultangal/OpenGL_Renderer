#include "pch.h"
#include "FrameBufferTexture.h"
#include "ErrorCheck.h"

FrameBufferTexture::FrameBufferTexture(const unsigned int& frameWidth, const unsigned int& frameHeight, const unsigned char& aaSamples)
{
    m_TexType = GL_TEXTURE_2D_MULTISAMPLE;

    GLCall(glGenTextures(1, &m_TextureID));
    AssignTexSlot(m_TextureSlot);
    GLCall(glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_TextureID));
    GLCall(glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, aaSamples, GL_RGB32F, frameWidth, frameHeight, GL_TRUE));
    GLCall(glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0));
    GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, m_TextureID, 0));    
}

FrameBufferTexture::FrameBufferTexture(const unsigned int& frameWidth, const unsigned int& frameHeight)
{
    m_TexType = GL_TEXTURE_2D;

    GLCall(glGenTextures(1, &m_TextureID));
    AssignTexSlot(m_TextureSlot);
    GLCall(glBindTexture(GL_TEXTURE_2D, m_TextureID));
    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, frameWidth, frameHeight, 0, GL_RGB, GL_FLOAT, NULL));
    //GLCall(glGenerateMipmap(GL_TEXTURE_2D));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    //GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 13));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_TextureID, 0));
}

FrameBufferTexture::~FrameBufferTexture()
{
    GLCall(glDeleteTextures(1, &m_TextureID));
}
