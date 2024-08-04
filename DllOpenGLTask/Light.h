#pragma once
#include <glm/glm.hpp>
#include "dllFile.h"

namespace openGLTask 
{
	struct OPENGLTASK_API SLight
	{
		glm::vec3 _LightColor;
		float _LightIntensity;
		glm::vec3 _LightPos;
	};

	struct OPENGLTASK_API SDirectionalLight : SLight
	{
		glm::vec3 _LightDir;
	};

	struct OPENGLTASK_API SPointLight : SLight
	{
		float _Near = 0.1f; // shadow frustum
		float _Far = 100.0f; // shadow frustum
		float _Radius;
	};
}


