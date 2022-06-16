#pragma once


class Texture
{
private:
	unsigned int m_TextureID;
	int m_TexType;
	unsigned char m_TextureSlot;
protected:
	void AssignTexSlot(unsigned char textureSlot);
	
public:
	Texture();
	Texture(const unsigned int& cubeMapWidth, const unsigned int& cubeMapHeight, bool genMipMap);
	Texture(const std::string& filePath, bool gammaCorrected);
	Texture(std::string hdrPath);
	Texture(const unsigned int& frameWidth, const unsigned int& frameHeight, const unsigned char& aaSamples);
	Texture(const unsigned int& frameWidth, const unsigned int& frameHeight);
	Texture(const unsigned int& frameWidth, const unsigned int& frameHeight, bool b, float f);
	~Texture();

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

