#pragma once
#include "GL/glew.h"
#include <string>

namespace Utils{

struct ShaderSource
{
	uint32_t type;
	std::string source;
};

class Shader
{
public:
	Shader();
	Shader(const std::string& vertexShader, const std::string& fragmentShader);
	~Shader();

	GLuint CreateShader(const std::string& vertexShader, const std::string& fragmentShader);

	GLuint const ID() { return m_ShaderID; }

	void Bind();
	void Unbind();

private:

	GLuint m_ShaderID;

	ShaderSource ParseShader(const std::string& filepath);
	GLuint CompileShader(const ShaderSource& shaderSource);

};

}
