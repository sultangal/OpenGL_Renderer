#include "pch.h"
#include "FrameBuffer.h"
#include "ErrorCheck.h"

FrameBuffer::FrameBuffer()
{
    GLCall(glGenFramebuffers(1, &m_RendererID));
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID));
}

FrameBuffer::~FrameBuffer() {
    GLCall(glDeleteBuffers(1, &m_RendererID));
}

void FrameBuffer::Bind() const {
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID));
}

void FrameBuffer::Blit(const unsigned int& frameWidth, const unsigned int& frameHeight, const unsigned char& aaSamples) const {
    GLCall(glBlitFramebuffer(0, 0, frameWidth, frameHeight, 0, 0, frameWidth, frameHeight, GL_COLOR_BUFFER_BIT, GL_NEAREST));
}

void FrameBuffer::CheckComplitness() const {
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
}


void FrameBuffer::BindRead() const {
    GLCall(glBindFramebuffer(GL_READ_FRAMEBUFFER, m_RendererID));
}

void FrameBuffer::BindDraw() const {
    GLCall(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_RendererID));
}

void FrameBuffer::BindDefaultFB() const {
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}
