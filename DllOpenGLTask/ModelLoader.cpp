#include "pch.h"
#include "ModelLoader.h"
#include "Mesh.h"
#include "GameObject.h"
#include "VertexBuffer.h"

#define TINYGLTF_MODE_DEFAULT -1 
#define TINYGLTF_MODE_POINT 0
#define TINYGLTF_MODE_TRIANGLE 4 
#define TINYGLTF_COMPONETTYPE_UNSHORT 5123
#define TINYGLTF_COMPONETTYPE_UNINT   5125

namespace openGLTask
{
	bool loadGLTF(const std::string& vFilename, tinygltf::Model& vModelGLTF)
	{
		tinygltf::TinyGLTF Loader;
		std::string Err;
		std::string Warn;

		bool res = Loader.LoadASCIIFromFile(&vModelGLTF, &Err, &Warn, vFilename);

		if (!Warn.empty()) {
			HIVE_LOG_WARNING("WARN: {}", Warn);
		}

		if (!Err.empty()) {
			HIVE_LOG_ERROR("ERR: {}", Err);
		}

		if (!res) {
			HIVE_LOG_ERROR("Failed to load glTF: {}", vFilename);
		}
		else {
			HIVE_LOG_INFO("Loaded glTF: {}", vFilename);
		}

		return res;
	}

	void createIndiceBufferData(std::vector<unsigned int>& vIndices, const tinygltf::BufferView& vBufferView, const tinygltf::Buffer& vBuffer, const int& vComponentType)
	{
		unsigned short tempUShortIndice;
		unsigned int   tempUIntIndice;
		const int UnShortByte = 2;
		const int UnIntByte = 4;
		if (vComponentType == TINYGLTF_COMPONETTYPE_UNSHORT)
		{
			for (size_t i = vBufferView.byteOffset; i < vBufferView.byteOffset + vBufferView.byteLength; i += UnShortByte) {
				std::memcpy(&tempUShortIndice, &vBuffer.data.at(i), sizeof(unsigned short));
				vIndices.push_back(tempUShortIndice);
			}
		}
		else if (vComponentType == TINYGLTF_COMPONETTYPE_UNINT)
		{
			for (size_t i = vBufferView.byteOffset; i < vBufferView.byteOffset + vBufferView.byteLength; i += UnIntByte) {
				std::memcpy(&tempUIntIndice, &vBuffer.data.at(i), sizeof(unsigned int));
				vIndices.push_back(tempUIntIndice);
			}
		}
	}

	void createVertexBufferData(std::vector<float>& vVertices, const tinygltf::Buffer& vBuffer, const int vIndex) {
		float tempVertice;
		const int FloatByte = 4;
		const int FloatNum = 3;
		for (auto i = vIndex; i < vIndex + FloatNum * FloatByte; i += FloatByte)
		{
			std::memcpy(&tempVertice, &vBuffer.data.at(i), sizeof(float));
			vVertices.push_back(tempVertice);
		}
	}

	void setCNodeTransform(std::shared_ptr<CGameObject> vPNode, const tinygltf::Node& vModelNode) 
{
		if (vModelNode.translation.size() == 3) {
			glm::vec3 tempPosition = glm::vec3((float)vModelNode.translation[0], (float)vModelNode.translation[1], (float)vModelNode.translation[2]);
			vPNode->setPosition(tempPosition);
		}
		if (vModelNode.rotation.size() == 4) {
			glm::quat tempRotationQuat = glm::quat((float)vModelNode.rotation[3], (float)vModelNode.rotation[0], (float)vModelNode.rotation[1], (float)vModelNode.rotation[2]);
			glm::vec3 euler = glm::eulerAngles(tempRotationQuat);
			glm::vec3 tempRotation = glm::degrees(euler);
			vPNode->setRotation(tempRotation);
		}
		if (vModelNode.scale.size() == 3) {
			glm::vec3 tempScale = glm::vec3((float)vModelNode.scale[0], (float)vModelNode.scale[1], (float)vModelNode.scale[2]);
			vPNode->setScale(tempScale);
		}
}


	void createCGameObject(tinygltf::Model& vGLTFModel, std::shared_ptr<CGameObject> vParentGameObject)
	{
		std::vector<float> Vertices;
		std::vector<unsigned int> Indices;
		for (auto& node : vGLTFModel.nodes) {
			if (node.mesh == -1) continue;
			const auto& mesh = vGLTFModel.meshes[node.mesh];
			std::string MeshName = mesh.name;
			HIVE_LOG_INFO("MeshName : {}", MeshName);
			const auto& pChildGameObject = std::make_shared<CGameObject>();

			for (auto& primitive : mesh.primitives) {
				Vertices.clear();
				if (primitive.mode == TINYGLTF_MODE_TRIANGLE or primitive.mode == TINYGLTF_MODE_DEFAULT) {
					Vertices.clear();
					Indices.clear();
					const tinygltf::BufferView& bufferViewInd = vGLTFModel.bufferViews[vGLTFModel.accessors[primitive.indices].bufferView];
					const tinygltf::Buffer& bufferInd = vGLTFModel.buffers[bufferViewInd.buffer];
					const int indiceComponentType = vGLTFModel.accessors[primitive.indices].componentType;

					createIndiceBufferData(Indices, bufferViewInd, bufferInd, indiceComponentType);
					HIVE_LOG_INFO("indice.size : {}", Indices.size());
					assert(Indices.size() == vGLTFModel.accessors[primitive.indices].count);

					const tinygltf::BufferView& bufferViewPos = vGLTFModel.bufferViews[vGLTFModel.accessors[primitive.attributes.at("POSITION")].bufferView];
					const tinygltf::Buffer& bufferPos = vGLTFModel.buffers[bufferViewPos.buffer];
					const tinygltf::BufferView& bufferViewNor = vGLTFModel.bufferViews[vGLTFModel.accessors[primitive.attributes.at("NORMAL")].bufferView];
					const tinygltf::Buffer& bufferNor = vGLTFModel.buffers[bufferViewNor.buffer];

					assert(bufferViewPos.byteLength == bufferViewNor.byteLength);

					const int vec3Byte = 12;
					for (size_t i = bufferViewPos.byteOffset, k = bufferViewNor.byteOffset;
						(i < bufferViewPos.byteOffset + bufferViewPos.byteLength && k < bufferViewNor.byteOffset + bufferViewNor.byteLength);
						i += vec3Byte, k += vec3Byte) {

						createVertexBufferData(Vertices, bufferPos, (int)i);
						createVertexBufferData(Vertices, bufferNor, (int)k);
					}
					HIVE_LOG_INFO("Vertices.size : {}", Vertices.size());
					assert(Vertices.size() == vGLTFModel.accessors[primitive.attributes.at("POSITION")].count * 6);
					const auto& pVAO = std::make_shared<CVertexBuffer>(Vertices, Indices, std::vector<int>{3, 3});
					const auto& pModel = std::make_shared<CMesh>(pVAO);
					pChildGameObject->addMesh(pModel);
					setCNodeTransform(pChildGameObject, node);
				}
			}
			vParentGameObject->addChild(pChildGameObject);
		}
		return;
	}

	std::shared_ptr<CGameObject> CModelLoader::loadGltfFile(const std::string& vFileName)
	{
		tinygltf::Model GLTFModel;
		const auto& pParentGameObject = std::make_shared<CGameObject>();
		if (!loadGLTF(vFileName, GLTFModel)) return nullptr;
		createCGameObject(GLTFModel, pParentGameObject);
		return pParentGameObject;
	}
}