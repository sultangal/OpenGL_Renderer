#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "pch.h"
#include "Bloom_Texture.h"
#include "ErrorCheck.h"

Bloom_Texture::Bloom_Texture()
{
    BloomMip bloomMip = { FRAME_WIDTH , FRAME_HEIGHT, 0 };
    int mipCount = 0;
    int resoulution = FRAME_HEIGHT;
    while (resoulution>3)
    {
        resoulution *= 0.5;
        mipCount++;
    }
    for (unsigned int i = 0; i < mipCount; i++)
    {
        GLCall(glGenTextures(1, &bloomMip.mipId));
        GLCall(glActiveTexture(GL_TEXTURE0));
        GLCall(glBindTexture(GL_TEXTURE_2D, bloomMip.mipId));
        GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_R11F_G11F_B10F, bloomMip.mipWidth, bloomMip.mipHeight, 0, GL_RGB, GL_FLOAT, nullptr));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
        m_MipIDs.push_back(bloomMip);
        bloomMip.mipWidth /= 2;
        bloomMip.mipHeight /= 2;
    }
    GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_MipIDs[0].mipId, 0));
}

Bloom_Texture::~Bloom_Texture(){}



void Bloom_Texture::GetMipChainTexIDs(std::vector<BloomMip>& mipIDs)
{
    mipIDs = m_MipIDs;
}

void Bloom_Texture::AttachTexToCurrFB(int texID)
{
    GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texID, 0));
}

void Bloom_Texture::Bind(unsigned char textureSlot, int texID)
{
    AssignTexSlot(textureSlot);
    GLCall(glBindTexture(GL_TEXTURE_2D, texID));
}

