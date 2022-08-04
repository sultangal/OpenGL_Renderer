#pragma once
#include "Texture.h"

class FrameBufferTexture : public Texture
{
public:
	FrameBufferTexture(const unsigned int& frameWidth, const unsigned int& frameHeight, const unsigned char& aaSamples);
	FrameBufferTexture(const unsigned int& frameWidth, const unsigned int& frameHeight);
	~FrameBufferTexture();
};

