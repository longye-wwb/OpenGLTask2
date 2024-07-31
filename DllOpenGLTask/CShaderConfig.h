#pragma once
#include "RenderConfiguration.h"

namespace openGLTask
{
	class CShaderConfig
	{
	public:
		CShaderConfig();
		~CShaderConfig() = default;

		const std::string& const getPixelVertShaderPath() { return m_PixelVertShaderPath; }
		const std::string& const getPixelFragShaderPath() { return m_PixelFragShaderPath; }
		const std::string& const getVertexVertShaderPath() { return m_VertexVertShaderPath; }
		const std::string& const getVertexFragShaderPath() { return m_VertexFragShaderPath; }
		void parseConfig(CRenderConfiguration& vConfig);

	private:
		void __checkAndSetPixelShader(std::optional<std::string> vPixelVertShaderPath, std::optional<std::string> vPixelFragShaderPath);
		void __checkAndSetVertexShader(std::optional<std::string> vVertexVertShaderPath, std::optional<std::string> vVertexFragShaderPath);

		std::string m_PixelVertShaderPath;
		std::string m_PixelFragShaderPath;
		std::string m_VertexVertShaderPath;
		std::string m_VertexFragShaderPath;
	};
}


