#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "dllFile.h"

namespace openGLTask
{
    class OPENGLTASK_API CShader
    {
    public:
        unsigned int m_ID;

        CShader(const char* vVertexPath, const char* vFragmentPath);

        void use() const;
        void setInt(const std::string& vName, int vValue) const;
        void setFloat(const std::string& vName, float vValue) const;
        void setVec3(const std::string& vName, const glm::vec3& vValue) const;
        void setVec3(const std::string& vName, float vPosx, float vPosy, float vPosz) const;
        void setMat4(const std::string& vName, const glm::mat4& vMat) const;
    private:
        void __checkCompileErrors(GLuint vShader, std::string vType);

    };

}
