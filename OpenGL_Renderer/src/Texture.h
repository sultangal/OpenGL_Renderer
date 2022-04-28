#pragma once


class Texture
{
private:
	unsigned int m_RendererID;
	unsigned char m_TextureSlot;
	int m_TexType;
	void AssignTexSlot(unsigned char textureSlot);
	
public:
	Texture(const std::string& filePath, unsigned char textureSlot, bool gammaCorrected);
	Texture(std::string faces[6], unsigned char textureSlot);
	Texture(const unsigned int& frameWidth, const unsigned int& frameHeight, const unsigned char& aaSamples, unsigned char textureSlot);
	Texture(const unsigned int& frameWidth, const unsigned int& frameHeight, unsigned char textureSlot);
	~Texture();

	void Bind(unsigned char textureSlot);
	void Bind();
	void Unbind();
	unsigned char GetTexSlotID();
};

