#include "shader_s.h"
#include <fstream>
#include <sstream>
#include "HiveLogger.h"
#include "safe.h"

namespace openGLTask
{
	CShader::CShader(const std::string& vVertShaderPath, const std::string& vFragShaderPath)
	{
		m_ShaderID = glCreateProgram();
		GLuint VertID = __attachShader(vVertShaderPath, EShaderType::VERTEX);
		GLuint FragID = __attachShader(vFragShaderPath, EShaderType::FRAGMENT);
		GL_SAFE_CALL(glAttachShader(m_ShaderID, VertID));
		GL_SAFE_CALL(glAttachShader(m_ShaderID, FragID));
		GL_SAFE_CALL(glLinkProgram(m_ShaderID));
		__checkLinkError(m_ShaderID);
		GL_SAFE_CALL(glDeleteShader(VertID));
		GL_SAFE_CALL(glDeleteShader(FragID));
	}

	CShader::CShader(const std::string& vVertShaderPath, const std::string& vFragShaderPath, const std::string& vGeometryShaderPath)
	{
		m_ShaderID = glCreateProgram();
		GLuint VertID = __attachShader(vVertShaderPath, EShaderType::VERTEX);
		GLuint FragID = __attachShader(vFragShaderPath, EShaderType::FRAGMENT);
		GLuint GeomID = __attachShader(vGeometryShaderPath, EShaderType::GEOMETRY);
		GL_SAFE_CALL(glAttachShader(m_ShaderID, VertID));
		GL_SAFE_CALL(glAttachShader(m_ShaderID, FragID));
		GL_SAFE_CALL(glAttachShader(m_ShaderID, GeomID));
		GL_SAFE_CALL(glLinkProgram(m_ShaderID));
		__checkLinkError(m_ShaderID);
		GL_SAFE_CALL(glDeleteShader(VertID));
		GL_SAFE_CALL(glDeleteShader(FragID));
		GL_SAFE_CALL(glDeleteShader(GeomID));
	}

	void CShader::use() const
	{
		GL_SAFE_CALL(glUseProgram(m_ShaderID));
	}

	void CShader::setUniform(const std::string& vName, int vValue) const
	{
		GL_SAFE_CALL(glUniform1i(glGetUniformLocation(m_ShaderID, vName.c_str()), vValue));
	}

	void CShader::setUniform(const std::string& vName, bool vValue) const
	{
		GL_SAFE_CALL(glUniform1i(glGetUniformLocation(m_ShaderID, vName.c_str()), (int)vValue));
	}

	void CShader::setUniform(const std::string& vName, float vValue) const
	{
		GL_SAFE_CALL(glUniform1f(glGetUniformLocation(m_ShaderID, vName.c_str()), vValue));
	}

	void CShader::setUniform(const std::string& vName, const glm::vec3& vValue) const
	{
		GL_SAFE_CALL(glUniform3fv(glGetUniformLocation(m_ShaderID, vName.c_str()), 1, &vValue[0]));
	}

	void CShader::setUniform(const std::string& vName, const glm::vec4& vValue) const
	{
		GL_SAFE_CALL(glUniform4fv(glGetUniformLocation(m_ShaderID, vName.c_str()), 1, &vValue[0]));
	}

	void CShader::setUniform(const std::string& vName, const glm::mat4& vMat) const
	{
		GL_SAFE_CALL(glUniformMatrix4fv(glGetUniformLocation(m_ShaderID, vName.c_str()), 1, GL_FALSE, &vMat[0][0]));
	}

	void CShader::setUniform(const std::string& vName, const glm::mat3& vMat) const
	{
		GL_SAFE_CALL(glUniformMatrix3fv(glGetUniformLocation(m_ShaderID, vName.c_str()), 1, GL_FALSE, &vMat[0][0]));
	}

	GLuint CShader::__attachShader(const std::string& vShaderPath, EShaderType vShaderType) const
	{
		std::string ShaderCode;
		__dumpShaderCodeFromFile(vShaderPath, vShaderType, ShaderCode);
		return __compileShader(ShaderCode, vShaderType);
	}

	void CShader::__dumpShaderCodeFromFile(const std::string& vShaderPath, EShaderType vShaderType, std::string& voShaderCode)
	{
		std::ifstream ShaderFile;
		ShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try
		{
			ShaderFile.open(vShaderPath);
			std::stringstream ShaderStream;
			ShaderStream << ShaderFile.rdbuf();
			ShaderFile.close();
			voShaderCode = ShaderStream.str();
		}
		catch (std::ifstream::failure& Error)
		{
			HIVE_LOG_ERROR("{} shader file READ failed: {}\n{}", __getShaderTypeName(vShaderType), vShaderPath, Error.what());
		}
	}

	GLuint CShader::__compileShader(const std::string& vShaderCode, EShaderType vShaderType)
	{
		GLuint ShaderID = glCreateShader(static_cast<GLenum>(vShaderType));
		const char* ShaderCodeCStr = vShaderCode.c_str();
		glShaderSource(ShaderID, 1, &ShaderCodeCStr, nullptr);
		glCompileShader(ShaderID);
		__checkCompileError(ShaderID, vShaderType);
		return ShaderID;
	}

	void CShader::__checkCompileError(GLuint vID, EShaderType vShaderType)
	{
		GLint IsSuccess;
		GLchar InfoLog[1024];
		glGetShaderiv(vID, GL_COMPILE_STATUS, &IsSuccess);
		if (!IsSuccess)
		{
			glGetShaderInfoLog(vID, 1024, nullptr, InfoLog);
			HIVE_LOG_ERROR("shader COMPILE failed: {}\n{}", __getShaderTypeName(vShaderType), InfoLog);
		}

	}

	void CShader::__checkLinkError(GLuint vID)
	{
		GLint IsSuccess;
		GLchar InfoLog[1024];
		glGetProgramiv(vID, GL_LINK_STATUS, &IsSuccess);
		if (!IsSuccess)
		{
			glGetProgramInfoLog(vID, 1024, nullptr, InfoLog);
			HIVE_LOG_ERROR("shader LINK failed: \n{}", InfoLog);
		}
	}

	std::string CShader::__getShaderTypeName(EShaderType vShaderType)
	{
		if (vShaderType == EShaderType::VERTEX) return "VERTEX";
		if (vShaderType == EShaderType::FRAGMENT) return "FRAGMENT";
		if (vShaderType == EShaderType::GEOMETRY) return "GEOMETRY";
		return "UNDEFINED";
	}
}