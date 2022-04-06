#pragma once
#include <GL/glew.h>
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

class Renderer {
public:
	void Clear(glm::vec4 color);
	void DrawIB(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
	void DrawVB(const VertexArray& va, const VertexBuffer& vb, const Shader& shader) const;
};