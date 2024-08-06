#pragma once
#include "VertexBuffer.h"
#include <memory>
#include "dllFile.h"

//TODO:增加基础的数量，如 三角 圆形 正方体 圆柱体
namespace openGLTask 
{
	class OPENGLTASK_API CPrimitive
	{
	public:
		static std::shared_ptr<CVertexBuffer> createQuad();
		static std::shared_ptr<CVertexBuffer> createPlane();
	};

}
