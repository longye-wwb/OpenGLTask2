#pragma once
#include "VertexBuffer.h"
#include <memory>

//TODO:���ӻ������������� ���� Բ�� ������ Բ����
namespace openGLTask 
{
	class CPrimitive
	{
	public:
		static std::shared_ptr<CVertexBuffer> createQuad();
	};

}
