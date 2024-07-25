#include "pch.h"
#include "RenderConfiguration.h"
namespace openGLTask {
	CRenderConfiguration::CRenderConfiguration() {
		_overwriteProductSig("MY_CONFIG");
		__defineAttributesV();
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
	}

}

