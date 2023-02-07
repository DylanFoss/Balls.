#include "pch.h"
#include "Shader.h"

#include <fstream>

Shader::Shader()
	:m_ShaderID(0)
{
}

Shader::Shader(const std::string& vertexShader, const std::string& fragmentShader)
	:m_ShaderID(0)
{
	CreateShader(vertexShader, fragmentShader);
}

Shader::~Shader()
{
	glDeleteProgram(m_ShaderID);
}

GLuint Shader::CreateShader(const std::string & vertexShader, const std::string & fragmentShader)
{
	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(ParseShader(vertexShader));
	unsigned int fs = CompileShader(ParseShader(fragmentShader));

	glAttachShader(program, vs);
	glAttachShader(program, fs);

	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	m_ShaderID = program;

	return m_ShaderID;
}

ShaderSource Shader::ParseShader(const std::string & filepath)
{
	std::ifstream stream(filepath);

	std::string line;
	std::stringstream ss;
	uint32_t type = GL_INVALID_ENUM;

	while (std::getline(stream, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
				type = GL_VERTEX_SHADER;
			else if (line.find("fragment") != std::string::npos)
				type = GL_FRAGMENT_SHADER;
		}
		else
		{
			ss << line << '\n';
		}
	}

	return { type, ss.str() };
}

GLuint Shader::CompileShader(const ShaderSource &shaderSource)
{
	GLuint shader = glCreateShader(shaderSource.type);
	const char* src = shaderSource.source.c_str();
	glShaderSource(shader, 1, &src, nullptr);

	glCompileShader(shader);

	//error handling
	GLint isCompiled;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE)
	{
		int length;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

		std::string message;
		message.resize(length);
		glGetShaderInfoLog(shader, length, &length, &message[0]);

		std::cout << "Failed to compile " << (shaderSource.type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!\n";
		std::cout << message << '\n';

		glDeleteShader(shader);
	}

	return shader;
}


void Shader::Bind()
{
	glUseProgram(m_ShaderID);
}

void Shader::Unbind()
{
	glUseProgram(0);
}
