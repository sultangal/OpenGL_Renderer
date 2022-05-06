#pragma once


class Texture
{
private:
	unsigned int m_RendererID;
	unsigned char m_TextureSlot;
	int m_TexType;
	void AssignTexSlot(unsigned char textureSlot);
	
public:
	Texture(const unsigned int& frameWidth, const unsigned int& frameHeight, int gl_tex_min_filter);
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
};

