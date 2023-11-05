// shader.cpp
#include "shader.h"
#include "../ew/external/glad.h"

namespace akcGPR
{
	std::string loadShaderSourceFromFile(const std::string& filePath)
	{
		std::ifstream fstream(filePath);

		if(!fstream.is_open())
		{
			printf("Failed to load file %s\n", filePath);
			return {};
		}

		std::stringstream buffer;
		buffer << fstream.rdbuf();
		return buffer.str();
	}

	unsigned int createShader(GLenum shaderType, const char* sourceCode)
	{
		// create and compile vertex shader object
		unsigned int shader = glCreateShader(shaderType);
		glShaderSource(shader, 1, &sourceCode, NULL);
		glCompileShader(shader);

		int success;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if(!success)
		{
			char infoLog[512];
			glGetShaderInfoLog(shader, 512, NULL, infoLog);
			printf("Failed to compile shader: %s", infoLog);
		}

		return shader;
	}

	unsigned int createShaderProgram(const char* vShaderSource, const char* fShaderSource)
	{
		unsigned int vShader = createShader(GL_VERTEX_SHADER, vShaderSource);
		unsigned int fShader = createShader(GL_FRAGMENT_SHADER, fShaderSource);
		unsigned int shaderProgram = glCreateProgram();

		// attach shaders to program
		glAttachShader(shaderProgram, vShader);
		glAttachShader(shaderProgram, fShader);

		// link all stages
		glLinkProgram(shaderProgram);

		int success;
		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
		if(!success)
		{
			char infoLog[512];
			glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
			printf("Failed to link shader program: %s\n", infoLog);
		}

		// delete vertex and fragment shader objects now that they are unneeded
		glDeleteShader(vShader);
		glDeleteShader(fShader);

		return shaderProgram;
	}

	// Shader class
	Shader::Shader(const std::string& vertexShader, const std::string& fragmentShader)
	{
		std::string vertexShaderSource = loadShaderSourceFromFile(vertexShader.c_str());
		std::string fragmentShaderSource = loadShaderSourceFromFile(fragmentShader.c_str());
		m_id = createShaderProgram(vertexShaderSource.c_str(), fragmentShaderSource.c_str());
	}

	void Shader::use()
	{
		glUseProgram(m_id);
	}

	void Shader::setInt(const std::string& name, int v) const
	{
		glUniform1i(glGetUniformLocation(m_id, name.c_str()), v);
	}

	void Shader::setFloat(const std::string& name, float v) const
	{
		glUniform1f(glGetUniformLocation(m_id, name.c_str()), v);
	}

	void Shader::setVec2(const std::string& name, float x, float y) const
	{
		glUniform2f(glGetUniformLocation(m_id, name.c_str()), x, y);
	}

	void Shader::setVec3(const std::string& name, float x, float y, float z) const
	{
		glUniform3f(glGetUniformLocation(m_id, name.c_str()), x, y, z);
	}

	void Shader::setVec4(const std::string& name, float x, float y, float z, float w) const
	{
		glUniform4f(glGetUniformLocation(m_id, name.c_str()), x, y, z, w);
	}

	void Shader::setMat4(const std::string& name, const ew::Mat4& v)
	{
		glUniformMatrix4fv(glGetUniformLocation(m_id, name.c_str()), 1, GL_FALSE, &v[0][0]);
	}
}