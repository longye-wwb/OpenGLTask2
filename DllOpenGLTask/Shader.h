#ifndef SHADER_S_H
#define SHADER_S_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

namespace openGLTask
{
    class Shader
    {
    private:
        void __checkCompileErrors(GLuint vShader, std::string vType);

    public:
        unsigned int m_ID;

        Shader(const char* vVertexPath, const char* vFragmentPath);

        void use() const;
        void setInt(const std::string& vName, int vValue) const;
        void setFloat(const std::string& vName, float vValue) const;
        void setVec3(const std::string& vName, const glm::vec3& vValue) const;
        void setVec3(const std::string& vName, float vPosx, float vPosy, float vPosz) const;
        void setMat4(const std::string& vName, const glm::mat4& vMat) const;
    };
}
#endif