#pragma once
#include "VertexBuffer.h"
#include <memory>
#include "dllFile.h"

//TODO:���ӻ������������� ���� Բ�� ������ Բ����
namespace openGLTask 
{
	class OPENGLTASK_API CPrimitive
	{
	public:
		static std::shared_ptr<CVertexBuffer> createQuad();
		static std::shared_ptr<CVertexBuffer> createPlane();
	};

}
