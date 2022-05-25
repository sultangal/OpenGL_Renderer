#pragma once
#include <GL/glew.h>
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

class Renderer {

public:
	Renderer();
	~Renderer();
	bool Init();
	void Clear(glm::vec4 color);
	void Clear();
	void DrawIB(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
	void DrawVB(const VertexArray& va, const VertexBuffer& vb, const Shader& shader) const;
	void DrawVB(const VertexArray& va, const VertexBuffer& vb) const;
	void DrawVB_Strip(const VertexArray& va, const VertexBuffer& vb) const;
	int GetWidth();
	int GetHeight();
	int GetAASamples();
	bool IsWindowClosed();
	void ConfigureViewport(unsigned int width, unsigned int height);
	void ProceedInput();
	void Terminate();
	void SwapBuffers();

	glm::vec3 GetCameraPos();
	glm::vec3 GetCameraFront();
	glm::vec3 GetCameraUp();
	float GetFov();
private:
	//void processInput(GLFWwindow* window);
	//void mouse_callback(GLFWwindow* window, double xpos, double ypos);
	//void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	//void framebuffer_size_callback(GLFWwindow* window, int width, int height);
};