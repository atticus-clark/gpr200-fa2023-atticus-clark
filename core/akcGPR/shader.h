// shader.h
#pragma once

#include <sstream>
#include <fstream>
#include "../ew/ewMath/mat4.h"

namespace akcGPR
{
	std::string loadShaderSourceFromFile(const std::string& filePath);

	class Shader
	{
	public:
		Shader(const std::string& vShader, const std::string& fShader);

		void use();
		void setInt(const std::string& name, int v) const;
		void setFloat(const std::string& name, float v) const;
		void setVec2(const std::string& name, float x, float y) const;
		void setVec3(const std::string& name, float x, float y, float z) const;
		void setVec4(const std::string& name, float x, float y, float z, float w) const;
		void setMat4(const std::string& name, const ew::Mat4& v);

	private:
		unsigned int m_id; // OpenGL program handle
	};
}