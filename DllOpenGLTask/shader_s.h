#ifndef SHADER_S_H
#define SHADER_S_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>

namespace openGLTask
{
	class CShader
	{
	public:
		enum class EShaderType
		{
			VERTEX = GL_VERTEX_SHADER,
			FRAGMENT = GL_FRAGMENT_SHADER,
			GEOMETRY = GL_GEOMETRY_SHADER,
		};

		CShader(const std::string& vVertShaderPath, const std::string& vFragShaderPath);
		CShader(const std::string& vVertShaderPath, const std::string& vFragShaderPath, const std::string& vGeometryShaderPath);
		void use() const;
		void setUniform(const std::string& vName, int vValue) const;
		void setUniform(const std::string& vName, bool vValue) const;
		void setUniform(const std::string& vName, float vValue) const;
		void setUniform(const std::string& vName, const glm::vec3& vValue) const;
		void setUniform(const std::string& vName, const glm::vec4& vValue) const;
		void setUniform(const std::string& vName, const glm::mat4& vMat) const;
		void setUniform(const std::string& vName, const glm::mat3& vMat) const;
	private:
		GLuint __attachShader(const std::string& vShaderPath, EShaderType vShaderType) const;
		static void __dumpShaderCodeFromFile(const std::string& vShaderPath, EShaderType vShaderType, std::string& voShaderCode);
		static GLuint __compileShader(const std::string& vShaderCode, EShaderType vShaderType);
		static void __checkCompileError(GLuint vID, EShaderType vShaderType);
		static void __checkLinkError(GLuint vID);
		static std::string __getShaderTypeName(EShaderType vShaderType);
		GLuint m_ShaderID;
	};
}
#endif