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

RenderBuffer::RenderBuffer(const unsigned int& frameWidth, const unsigned int& frameHeight)
{
    GLCall(glGenRenderbuffers(1, &m_RendererID));
    GLCall(glBindRenderbuffer(GL_RENDERBUFFER, m_RendererID));
    GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, frameWidth, frameHeight));
    GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_RendererID));
}

RenderBuffer::~RenderBuffer() {
    GLCall(glDeleteBuffers(1, &m_RendererID));
}

void RenderBuffer::Bind() const {
    GLCall(glBindRenderbuffer(GL_RENDERBUFFER, m_RendererID));
}

void RenderBuffer::Unbind() const {
    GLCall(glBindRenderbuffer(GL_RENDERBUFFER, 0));
}

void RenderBuffer::ConfigureRBStorage(const unsigned int& width, const unsigned int& height) {
    GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height));
}

