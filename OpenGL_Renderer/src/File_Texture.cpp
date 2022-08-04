#include "pch.h"
#include "File_Texture.h"
#include <GL/glew.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "ErrorCheck.h"

File_Texture::File_Texture(const std::string& filePath, bool gammaCorrected)
{
    m_TexType = GL_TEXTURE_2D;

    stbi_set_flip_vertically_on_load(true);
    int width, height, nrChannels;
    m_DataChar = stbi_load(filePath.c_str(), &width, &height, &nrChannels, 0);
    GLCall(glGenTextures(1, &m_TextureID));
    AssignTexSlot(m_TextureSlot);
    if (m_DataChar) {
        GLCall(glBindTexture(GL_TEXTURE_2D, m_TextureID));
        //GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
        if (gammaCorrected) {
            GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, m_DataChar));
        }
        else {
            GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, m_DataChar));
        }
        GLCall(glGenerateMipmap(GL_TEXTURE_2D));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
        std::cout << "[Texture][MESSAGE]::Texture : " << filePath << " - created." << std::endl;
    }
    else {
        std::cout << red << "[Texture][ERROR]::Failed to load texture" << white << std::endl;
    }
    stbi_image_free(m_DataChar);
}

File_Texture::File_Texture(std::string filePath)
{
    m_TexType = GL_TEXTURE_2D;

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    m_DataFloat = stbi_loadf(filePath.c_str(), &width, &height, &nrChannels, 0);
    if (m_DataFloat)
    {
        GLCall(glGenTextures(1, &m_TextureID));
        AssignTexSlot(m_TextureSlot);
        GLCall(glBindTexture(GL_TEXTURE_2D, m_TextureID));
        GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 0, GL_RGB, GL_FLOAT, m_DataFloat));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
       
        stbi_image_free(m_DataFloat);
        std::cout << "[Texture][MESSAGE]::Texture : " << filePath << " - created." << std::endl;
    }
    else
    {
        std::cout << red << "[Texture][ERROR]::Failed to load texture at path: " << white << filePath << std::endl;
        stbi_image_free(m_DataFloat);
    }   
}

File_Texture::~File_Texture()
{
    if (!m_DataFloat) stbi_image_free(m_DataFloat);
    if (!m_DataChar) stbi_image_free(m_DataChar);
    GLCall(glDeleteTextures(1, &m_TextureID));
}
