#include "pch.h"
#include "Shader.h"
#include "ErrorCheck.h"


Shader::Shader(const std::string& filepath)
	:m_FilePath(filepath), m_RendererID(0)
{
	ShaderProgramSource source = ParseShader(filepath);
	m_RendererID = CreateShader(source.VertexShader, source.FragmentShader);
}

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath)
	:m_VertexPath(vertexPath), m_FragmentPath(fragmentPath), m_RendererID(0)
{
	ShaderProgramSource source = ParseShader(m_VertexPath, m_FragmentPath);
	m_RendererID = CreateShader(source.VertexShader, source.FragmentShader);
}

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath, const std::string& geometryPath)
	:m_VertexPath(vertexPath), m_FragmentPath(fragmentPath), m_GeometryPath(geometryPath), m_RendererID(0)
{
	ShaderProgramSource source = ParseShader(m_VertexPath, m_FragmentPath, m_GeometryPath);
	m_RendererID = CreateShader(source.VertexShader, source.FragmentShader, source.GeometryShader);
}

Shader::~Shader()
{
	GLCall(glDeleteProgram(m_RendererID));
}

void Shader::Bind() const
{
	GLCall(glUseProgram(m_RendererID));
}

void Shader::Unbind() const
{
	GLCall(glUseProgram(0));
}

void Shader::SetUniform1f(const std::string& name, const float data)
{
	GLCall(glUniform1f(GetUniformLocation(name), data));
}

void Shader::SetUniform3f(const std::string& name, const float v0, const float v1, const float v2)
{
	GLCall(glUniform3f(GetUniformLocation(name), v0, v1, v2));
}

void Shader::SetUniform1i(const std::string& name, const int data)
{
	GLCall(glUniform1i(GetUniformLocation(name), data));
}

void Shader::SetUniformMatrix4fv(const std::string& name, const float* data)
{
	GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, data));
}

ShaderProgramSource Shader::ParseShader(const std::string& filepath)
{
	std::ifstream stream(filepath);

	enum class ShaderType
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;
	while (getline(stream, line)) {
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
				type = ShaderType::VERTEX;
			else if (line.find("fragment") != std::string::npos)
				type = ShaderType::FRAGMENT;
		}
		else {
			ss[(int)type] << line << '\n';
		}
	}
	return { ss[0].str(), ss[1].str() };
}

ShaderProgramSource Shader::ParseShader(const std::string& vertexPath, const std::string& fragmentPath)
{
	std::ifstream streamV(vertexPath);
	std::ifstream streamF(fragmentPath);
	std::string line;
	std::string srcV;
	std::string srcF;
	while (!streamV.eof()) {
		std::getline(streamV, line);
		srcV += line + '\n';
	}
	while (!streamF.eof()) {
		std::getline(streamF, line);
		srcF += line + '\n';
	}
	return { srcV, srcF };
}

ShaderProgramSource Shader::ParseShader(const std::string& vertexPath, const std::string& fragmentPath, const std::string& geometryPath)
{
	std::ifstream streamV(vertexPath);
	std::ifstream streamF(fragmentPath);
	std::ifstream streamG(geometryPath);
	std::string line;
	std::string srcV;
	std::string srcF;
	std::string srcG;
	while (!streamV.eof()) {
		std::getline(streamV, line);
		srcV += line + '\n';
	}
	while (!streamF.eof()) {
		std::getline(streamF, line);
		srcF += line + '\n';
	}
	while (!streamG.eof()) {
		std::getline(streamG, line);
		srcG += line + '\n';
	}
	return { srcV, srcF, srcG };
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
	GLCall(unsigned int id = glCreateShader(type));
	const char* src = source.c_str();
	GLCall(glShaderSource(id, 1, &src, nullptr));
	GLCall(glCompileShader(id));

	int result;
	GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
	if (result == GL_FALSE)
	{
		int length;
		GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
		char* message = (char*)_malloca(length * sizeof(char));
		GLCall(glGetShaderInfoLog(id, length, &length, message));
		std::cout << "Faled to compile " <<
			(type == GL_VERTEX_SHADER ? "vertex" : "fragment")
			<< " shader!" << std::endl;
		std::cout << message << std::endl;
		GLCall(glDeleteShader(id));
		return 0;
	}
	return id;
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	GLCall(unsigned int program = glCreateProgram());
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	GLCall(glAttachShader(program, vs));
	GLCall(glAttachShader(program, fs));
	GLCall(glLinkProgram(program));
	GLCall(glValidateProgram(program));

	GLCall(glDeleteShader(vs));
	GLCall(glDeleteShader(fs));

	return program;
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader, const std::string& geometryPath)
{
	GLCall(unsigned int program = glCreateProgram());
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);
	unsigned int gs = CompileShader(GL_GEOMETRY_SHADER, geometryPath);

	GLCall(glAttachShader(program, vs));
	GLCall(glAttachShader(program, fs));
	GLCall(glAttachShader(program, gs));
	GLCall(glLinkProgram(program));
	GLCall(glValidateProgram(program));

	GLCall(glDeleteShader(vs));
	GLCall(glDeleteShader(fs));
	GLCall(glDeleteShader(gs));

	return program;
}

unsigned int Shader::GetUniformLocation(const std::string& name)
{
	if(m_UniformLocationCashe.find(name) != m_UniformLocationCashe.end())
	return m_UniformLocationCashe[name];

	GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
	if (location == -1)
		std::cout << "Warning: uniform '" << name << "' does not exist!" << std::endl;
	m_UniformLocationCashe[name] = location;
	return location;
}
