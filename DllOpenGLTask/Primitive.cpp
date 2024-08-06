#include "Primitive.h"
#include "pch.h"

namespace openGLTask
{
	std::shared_ptr<CVertexBuffer> CPrimitive::createQuad()
	{
		std::vector<float> Vertices{
			// positions   // texCoords
			-1.0f,  1.0f,  0.0f, 1.0f,
			-1.0f, -1.0f,  0.0f, 0.0f,
			 1.0f, -1.0f,  1.0f, 0.0f,
			 1.0f,  1.0f,  1.0f, 1.0f
		};
		std::vector<unsigned int> Indices{
			0, 1, 2, // first Triangle
			0, 2, 3, // second Triangle
		};
		return std::make_shared<CVertexBuffer>(Vertices, Indices, std::vector<int>{2, 2});
	}	
	std::shared_ptr<CVertexBuffer> CPrimitive::createPlane()
	{
		const std::vector Vertices{
			// positions          // normals
			-0.5f, 0.0f, -0.5f,  0.0f, 1.0f, 0.0f,
			 0.5f, 0.0f, -0.5f,  0.0f, 1.0f, 0.0f,
			 0.5f, 0.0f,  0.5f,  0.0f, 1.0f, 0.0f,
			-0.5f, 0.0f,  0.5f,  0.0f, 1.0f, 0.0f
		};

		const std::vector<unsigned int> Indices{
			0, 1, 2,
			2, 3, 0
		};
		return  std::make_shared<CVertexBuffer>(Vertices, Indices, std::vector<int>{3, 3});
	}

}