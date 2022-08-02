#pragma once
#include "Texture.h"

class IBL_Texture : public Texture 
{	
public:
	IBL_Texture(const unsigned int& cubeMapWidth, const unsigned int& cubeMapHeight, bool genMipMap);
	IBL_Texture(const unsigned int& frameWidth, const unsigned int& frameHeight);
	~IBL_Texture();

	void AttachCubeTexToCurrFB(unsigned int i, unsigned int mipmapLevel);
};

