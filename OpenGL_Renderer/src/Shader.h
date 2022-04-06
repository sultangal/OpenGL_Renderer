#pragma once

struct ShaderProgramSource {
	std::string VertexShader;
	std::string FragmentShader;
	std::string GeometryShader;
};

class Shader
{
private:
	std::string m_FilePath;
	std::string m_VertexPath;
	std::string m_FragmentPath;
	std::string m_GeometryPath;
	unsigned int m_RendererID;
	std::unordered_map<std::string, unsigned int> m_UniformLocationCashe;

public:
	Shader(const std::string& filepath);
	Shader(const std::string& vertexPath, const std::string& fragmentPath);
	Shader(const std::string& vertexPath, const std::string& fragmentPath, const std::string& geometryPath);
	~Shader();
	void Bind() const;
	void Unbind() const;

	void SetUniform1f(const std::string& name, const float data);
	void SetUniform3f(const std::string& name, const float v0, const float v1, const float v2);
	void SetUniform1i(const std::string& name, const int data);
	void SetUniformMatrix4fv(const std::string& name, const float* data);
private:
	ShaderProgramSource ParseShader(const std::string& filepath);
	ShaderProgramSource ParseShader(const std::string& vertexPath, const std::string& fragmentPath);
	ShaderProgramSource ParseShader(const std::string& vertexPath, const std::string& fragmentPath, const std::string& geometryPath);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader, const std::string& geometryPath);
	unsigned int GetUniformLocation(const std::string& name);
};

