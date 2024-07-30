#include "RenderConfiguration.h"

namespace openGLTask 
{
	CRenderConfiguration::CRenderConfiguration() {
		_overwriteProductSig("MY_CONFIG");
		__defineAttributesV();
		__loadDefaultConfigV();
	}

	void CRenderConfiguration::__defineAttributesV()
	{
		_defineAttribute("Width", hiveConfig::EConfigDataType::ATTRIBUTE_INT);
		_defineAttribute("Height", hiveConfig::EConfigDataType::ATTRIBUTE_INT);
		_defineAttribute("MajorVersion", hiveConfig::EConfigDataType::ATTRIBUTE_INT);
		_defineAttribute("MinorVersion", hiveConfig::EConfigDataType::ATTRIBUTE_INT);
		_defineAttribute("PosX", hiveConfig::EConfigDataType::ATTRIBUTE_INT);
		_defineAttribute("PosY", hiveConfig::EConfigDataType::ATTRIBUTE_INT);
		_defineAttribute("WinName", hiveConfig::EConfigDataType::ATTRIBUTE_STRING);
		_defineAttribute("UseCoreProfile", hiveConfig::EConfigDataType::ATTRIBUTE_BOOL);
		
		_defineAttribute("SHADER",hiveConfig::EConfigDataType::ATTRIBUTE_SUBCONFIG);
		_defineAttribute("SHADER_SOURCE_FILE", hiveConfig::EConfigDataType::ATTRIBUTE_STRING);
		_defineAttribute("RENDER_ALGORITHM", hiveConfig::EConfigDataType::ATTRIBUTE_SUBCONFIG);
		_defineAttribute("RENDER_PASS", hiveConfig::EConfigDataType::ATTRIBUTE_SUBCONFIG);
		_defineAttribute("VERTEX_SHADER", hiveConfig::EConfigDataType::ATTRIBUTE_STRING);
		_defineAttribute("FRAGMENT_SHADER", hiveConfig::EConfigDataType::ATTRIBUTE_STRING);

		_defineAttribute("CameraPos", hiveConfig::EConfigDataType::ATTRIBUTE_VEC3F);
		_defineAttribute("CameraFront", hiveConfig::EConfigDataType::ATTRIBUTE_VEC3F);
		_defineAttribute("CameraUp", hiveConfig::EConfigDataType::ATTRIBUTE_VEC3F);
		_defineAttribute("LightDirection", hiveConfig::EConfigDataType::ATTRIBUTE_VEC3F);
		_defineAttribute("GLTFPATH", hiveConfig::EConfigDataType::ATTRIBUTE_STRING);
	}

	void CRenderConfiguration::__loadDefaultConfigV()
	{
		setAttribute("Width",800);
		setAttribute("Height",600);
		setAttribute("MajorVersion",3);
		setAttribute("MinorVersion",3);
		setAttribute("PosX",0);
		setAttribute("PosY",0);
		setAttribute("WinName",std::string("Learn_OpenGL"));
		setAttribute("UseCoreProfile",true);
		setAttribute("CameraPos",std::tuple<double, double, double>(0.0f, 0.0f, -3.0f));
		setAttribute("CameraFront",std::tuple<double, double, double>(0.0f, 0.0f, -1.0f));
		setAttribute("CameraUp",std::tuple<double, double, double>(0.0f, 1.0f, 0.0f));
		setAttribute("LightDirection",std::tuple<double, double, double>(0.0f, 0.0f, -1.0f));
		setAttribute("GLTFPATH",std::string("../models/dragon.gltf"));
	}
}

