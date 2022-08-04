#pragma once


class Texture
{
protected:
	unsigned int m_TextureID = 0;
	int m_TexType = 0;
	unsigned char m_TextureSlot = 0;
	
public:
	Texture();
	~Texture();

	int GetID();
	void Bind(unsigned char textureSlot);
	void Bind();
	void Unbind();
	unsigned char GetTexSlotID();
	void AttachTexToCurrFB();
	void AttachTexToCurrFB(unsigned int mipmapLevel);
	void GenMipMap();
	void AssignTexSlot(unsigned char textureSlot);
};

