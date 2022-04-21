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
	void Blit(const unsigned int& frameWidth, const unsigned int& frameHeight) const;
	void CheckComplitness() const;
	void Unbind() const;
	void EnableDepthTest() const;
	void DisableDepthTest() const;
};

