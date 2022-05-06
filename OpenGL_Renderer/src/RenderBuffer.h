#pragma once
class RenderBuffer
{
private:
	unsigned int m_RendererID;
public:
	RenderBuffer(const unsigned int &frameWidth, const unsigned int &frameHeight, const unsigned char &aaSamples);
	RenderBuffer(const unsigned int& frameWidth, const unsigned int& frameHeight);
	~RenderBuffer();
	void Bind() const;
	void BindDefaultFB() const;
};

