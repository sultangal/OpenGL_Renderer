#include "pch.h"
#include "Texture.h"
#include "ErrorCheck.h"

Texture::Texture() {}

void Texture::Bind(unsigned char textureSlot)
{
    m_TextureSlot = textureSlot;
    AssignTexSlot(textureSlot);
    //std::cout << "[Texture][MESSAGE]::Assigned texture slot to: " << (int)textureSlot << std::endl;
    GLCall(glBindTexture(m_TexType, m_TextureID));
}

void Texture::Bind()
{
    AssignTexSlot(m_TextureSlot);
    GLCall(glBindTexture(m_TexType, m_TextureID));
}

void Texture::Unbind()
{
    GLCall(glBindTexture(m_TexType, 0));
}

int Texture::GetID()
{
    return m_TextureID;
}

unsigned char Texture::GetTexSlotID()
{
    return m_TextureSlot;
}

void Texture::AssignTexSlot(unsigned char textureSlot) {
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

void Texture::AttachTexToCurrFB()
{
    GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_TexType, m_TextureID, 0));
}

void Texture::AttachTexToCurrFB(unsigned int mipmapLevel)
{
    GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_TexType, m_TextureID, mipmapLevel));
}

void Texture::GenMipMap()
{
    GLCall(glGenerateMipmap(m_TexType));
}

Texture::~Texture()
{
    GLCall(glDeleteTextures(1, &m_TextureID));
}
