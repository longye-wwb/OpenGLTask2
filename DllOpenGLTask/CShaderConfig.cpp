#include "CShaderConfig.h"
#include "HiveLogger.h"
#include <filesystem>

namespace openGLTask
{
	CShaderConfig::CShaderConfig() : m_PixelVertShaderPath("../shaders/vertPerPixelShading.glsl"), m_PixelFragShaderPath("../shaders/fragPerPixelShading.glsl"),
		m_VertexVertShaderPath("../shaders/vertPerVertexShading.glsl"), m_VertexFragShaderPath("../shaders/fragPerVertexShading.glsl")
	{
	}

	void CShaderConfig::parseConfig(CRenderConfiguration& vConfig)
	{
		std::optional<std::string> PixelVertShaderPath = vConfig.getAttribute<std::string>("shader_perpixel_shading_vs|SHADER_SOURCE_FILE").value();
		std::optional<std::string> PixelFragShaderPath = vConfig.getAttribute<std::string>("shader_perpixel_shading_fs|SHADER_SOURCE_FILE").value();
		__checkAndSetPixelShader(PixelVertShaderPath, PixelFragShaderPath);

		std::optional<std::string> VtexVertShaderPath = vConfig.getAttribute<std::string>("shader_pervertex_shading_vs|SHADER_SOURCE_FILE").value();
		std::optional<std::string> VtexFragShaderPath = vConfig.getAttribute<std::string>("shader_pervertex_shading_fs|SHADER_SOURCE_FILE").value();
		__checkAndSetVertexShader(VtexVertShaderPath, VtexFragShaderPath);
	}

	void CShaderConfig::__checkAndSetPixelShader(std::optional<std::string> vPixelVertShaderPath, std::optional<std::string> vPixelFragShaderPath)
	{
		if (vPixelVertShaderPath.has_value() && vPixelFragShaderPath.has_value() && std::filesystem::exists(vPixelVertShaderPath.value()) && std::filesystem::exists(vPixelFragShaderPath.value()))
		{
			m_PixelVertShaderPath = vPixelVertShaderPath.value();
			m_PixelFragShaderPath = vPixelFragShaderPath.value();
		}
		else
		{
			HIVE_LOG_ERROR("Pixel Shader path is invalid, use default path.");
		}
		return;
	}

	void CShaderConfig::__checkAndSetVertexShader(std::optional<std::string> vVertexVertShaderPath, std::optional<std::string> vVertexFragShaderPath)
	{
		if (vVertexVertShaderPath.has_value() && vVertexFragShaderPath.has_value() && std::filesystem::exists(vVertexVertShaderPath.value()) && std::filesystem::exists(vVertexFragShaderPath.value()))
		{
			m_VertexVertShaderPath = vVertexVertShaderPath.value();
			m_VertexFragShaderPath = vVertexFragShaderPath.value();
		}
		else
		{
			HIVE_LOG_ERROR("Vertex Shader path is invalid, use default path.");
		}
		return;
	}

}



