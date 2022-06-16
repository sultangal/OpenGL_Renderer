#pragma once


class FrameBufferTexture
{
private:
	unsigned int m_TextureID;
	int m_TexType;
	unsigned char m_TextureSlot;
protected:
	void AssignTexSlot(unsigned char textureSlot);
	
public:
	FrameBufferTexture();
	FrameBufferTexture(const unsigned int& frameWidth, const unsigned int& frameHeight, const unsigned char& aaSamples);
	FrameBufferTexture(const unsigned int& frameWidth, const unsigned int& frameHeight);
	FrameBufferTexture(const unsigned int& frameWidth, const unsigned int& frameHeight, bool b, float f);
	~FrameBufferTexture();

	int GetID();
	void Bind(unsigned char textureSlot);
	void Bind();
	void Unbind();
	unsigned char GetTexSlotID();
	void AttachTexToCurrFB();
	void AttachCubeTexToCurrFB(unsigned int i, unsigned int mipmapLevel);
	void AttachTexToCurrFB(unsigned int mipmapLevel);
	virtual void GenMipMap();
};

