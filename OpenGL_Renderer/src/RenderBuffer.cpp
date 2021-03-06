#include "pch.h"
#include "RenderBuffer.h"
#include "ErrorCheck.h"

RenderBuffer::RenderBuffer(const unsigned int& frameWidth, const unsigned int& frameHeight, const unsigned char& aaSamples)
{
    GLCall(glGenRenderbuffers(1, &m_RendererID));
    GLCall(glBindRenderbuffer(GL_RENDERBUFFER, m_RendererID));
    GLCall(glRenderbufferStorageMultisample(GL_RENDERBUFFER, aaSamples, GL_DEPTH24_STENCIL8, frameWidth, frameHeight));
    GLCall(glBindRenderbuffer(GL_RENDERBUFFER, 0));
    GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RendererID));
}

RenderBuffer::~RenderBuffer() {
    GLCall(glDeleteBuffers(1, &m_RendererID));
}

void RenderBuffer::Bind() const {
    GLCall(glBindRenderbuffer(GL_RENDERBUFFER, m_RendererID));
}

void RenderBuffer::BindDefaultFB() const {
    GLCall(glBindRenderbuffer(GL_RENDERBUFFER, 0));
}

