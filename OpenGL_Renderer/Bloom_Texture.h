#pragma once
#include "Texture.h"
struct BloomMip
{
    unsigned int mipWidth;
    unsigned int mipHeight;
    unsigned int mipId;
};

class Bloom_Texture : public Texture
{
public:
    Bloom_Texture();
    ~Bloom_Texture();

    void GetMipChainTexIDs(std::vector<BloomMip>& bloomMip);
    void AttachTexToCurrFB(int texID);
    void Bind(unsigned char textureSlot, int texID);

private:
    std::vector<BloomMip> m_MipIDs;
};

