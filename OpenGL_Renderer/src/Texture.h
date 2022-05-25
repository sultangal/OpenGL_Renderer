#pragma once


class Texture
{
private:
	unsigned int m_TextureID;
	unsigned char m_TextureSlot;
	int m_TexType;
	void AssignTexSlot(unsigned char textureSlot);
	
public:
	Texture(const unsigned int& cubeMapWidth, const unsigned int& cubeMapHeight, bool genMipMap);
	Texture(const std::string& filePath, bool gammaCorrected);
	Texture(std::string hdrPath);
	Texture(const unsigned int& frameWidth, const unsigned int& frameHeight, const unsigned char& aaSamples);
	Texture(const unsigned int& frameWidth, const unsigned int& frameHeight);
	~Texture();

	int GetID();
	void Bind(unsigned char textureSlot);
	void Bind();
	void Unbind();
	unsigned char GetTexSlotID();
	void AttachTexToCurrFB();
	void AttachCubeTexToCurrFB(unsigned int i, unsigned int mipmapLevel);
	void GenMipMap();
};

