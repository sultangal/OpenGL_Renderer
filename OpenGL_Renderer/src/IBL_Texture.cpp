#include "pch.h"
#include "IBL_Texture.h"
#include "ErrorCheck.h"

IBL_Texture::IBL_Texture(const unsigned int& cubeMapWidth, const unsigned int& cubeMapHeight, bool containMipMap)
{
    m_TexType = GL_TEXTURE_CUBE_MAP;

    GLCall(glGenTextures(1, &m_TextureID));
    AssignTexSlot(m_TextureSlot);
    GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, m_TextureID));
    for (unsigned int i = 0; i < 6; ++i)
    {
        GLCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, cubeMapWidth, cubeMapHeight, 0, GL_RGB, GL_FLOAT, nullptr));
    }   
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    if (containMipMap) {
        GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR)); // enable pre-filter mipmap sampling (combatting visible dots artifact)
    }
    else {
        GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    }
}



IBL_Texture::IBL_Texture(const unsigned int& frameWidth, const unsigned int& frameHeight)
{
    m_TexType = GL_TEXTURE_2D;

    GLCall(glGenTextures(1, &m_TextureID));
    AssignTexSlot(m_TextureSlot);
    GLCall(glBindTexture(GL_TEXTURE_2D, m_TextureID));
    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, frameWidth, frameHeight, 0, GL_RG, GL_FLOAT, NULL));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_TextureID, 0));   
}

void IBL_Texture::AttachCubeTexToCurrFB(unsigned int i, unsigned int mipmapLevel)
{
    GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X+i, m_TextureID, mipmapLevel));
}

IBL_Texture::~IBL_Texture()
{
    GLCall(glDeleteTextures(1, &m_TextureID));
}
