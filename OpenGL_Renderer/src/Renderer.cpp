#include "pch.h"
#include "Renderer.h"
#include "ErrorCheck.h"

void Renderer::Clear(glm::vec4 color){
    GLCall(glClearColor(color.r, color.g, color.b, color.a));
    GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));   
}


void Renderer::DrawIB(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const {
    shader.Bind();
    va.Bind();
    ib.Bind();
    GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::DrawVB(const VertexArray& va, const VertexBuffer& vb, const Shader& shader) const {
    shader.Bind();
    va.Bind();
    GLCall(glDrawArrays(GL_TRIANGLES, 0, vb.GetSize()/sizeof(float)));
}
