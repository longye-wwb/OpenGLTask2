#pragma once
#include "VertexBuffer.h"
#include <memory>

//TODO:增加基础的数量，如 三角 圆形 正方体 圆柱体
namespace openGLTask 
{
	class CPrimitive
	{
	public:
		static std::shared_ptr<CVertexBuffer> createQuad();
	};

}
