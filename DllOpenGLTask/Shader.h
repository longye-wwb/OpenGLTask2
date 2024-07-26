#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>

namespace openGLTask
{
    class CShader
    {
    private:
        void __checkCompileErrors(GLuint vShader, std::string vType);

    public:
        unsigned int m_ID;

        CShader(const char* vVertexPath, const char* vFragmentPath);

        void use() const;
        void setInt(const std::string& vName, int vValue) const;
        void setFloat(const std::string& vName, float vValue) const;
        void setVec3(const std::string& vName, const glm::vec3& vValue) const;
        void setVec3(const std::string& vName, float vPosx, float vPosy, float vPosz) const;
        void setMat4(const std::string& vName, const glm::mat4& vMat) const;
    };
}
