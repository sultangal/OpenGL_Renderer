#pragma once
class FrameBuffer
{
private:
	unsigned int m_RendererID;
public:
	FrameBuffer();
	~FrameBuffer();
	void Bind() const;
	void BindRead() const;
	void BindDraw() const;
	void Blit(const unsigned int& frameWidth, const unsigned int& frameHeight, const unsigned char& aaSamples) const;
	void CheckComplitness() const;
	void BindDefaultFB() const;
};

