#include "Shader.h"
#include <fstream>
#include <sstream>
#include <iostream>

namespace openGLTask
{
    CShader::CShader(const char* vVertexPath, const char* vFragmentPath)
    {
        std::string VertexCode;
        std::string FragmentCode;
        std::ifstream ShaderFileVertex;
        std::ifstream ShaderFileFragment;

        ShaderFileVertex.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        ShaderFileFragment.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try
        {

            ShaderFileVertex.open(vVertexPath);
            ShaderFileFragment.open(vFragmentPath);
            std::stringstream ShaderStreamVertex, ShaderStreamFragment;

            ShaderStreamVertex << ShaderFileVertex.rdbuf();
            ShaderStreamFragment << ShaderFileFragment.rdbuf();

            ShaderFileVertex.close();
            ShaderFileFragment.close();

            VertexCode = ShaderStreamVertex.str();
            FragmentCode = ShaderStreamFragment.str();
        }
        catch (std::ifstream::failure& e)
        {
            std::cout << "ERROR::CShader::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
        }
        const char* ShaderCodeVertex = VertexCode.c_str();
        const char* ShaderCodeFragment = FragmentCode.c_str();

        unsigned int Vertex, Fragment;

        Vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(Vertex, 1, &ShaderCodeVertex, NULL);
        glCompileShader(Vertex);
        __checkCompileErrors(Vertex, "Vertex");

        Fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(Fragment, 1, &ShaderCodeFragment, NULL);
        glCompileShader(Fragment);
        __checkCompileErrors(Fragment, "Fragment");

        m_ID = glCreateProgram();
        glAttachShader(m_ID, Vertex);
        glAttachShader(m_ID, Fragment);
        glLinkProgram(m_ID);
        __checkCompileErrors(m_ID, "PROGRAM");

        glDeleteShader(Vertex);
        glDeleteShader(Fragment);

    }

    void CShader::use() const
    {
        glUseProgram(m_ID);
    }

    void CShader::setInt(const std::string& vName, int vValue) const
    {
        glUniform1i(glGetUniformLocation(m_ID, vName.c_str()), vValue);
    }

    void CShader::setFloat(const std::string& vName, float vValue) const
    {
        glUniform1f(glGetUniformLocation(m_ID, vName.c_str()), vValue);
    }

    void CShader::setVec3(const std::string& vName, const glm::vec3& vValue) const
    {
        glUniform3fv(glGetUniformLocation(m_ID, vName.c_str()), 1, &vValue[0]);
    }

    void CShader::setVec3(const std::string& vName, float vPosx, float vPosy, float vPosz) const
    {
        glUniform3f(glGetUniformLocation(m_ID, vName.c_str()), vPosx, vPosy, vPosz);
    }

    void CShader::setMat4(const std::string& vName, const glm::mat4& vMat) const
    {
        glUniformMatrix4fv(glGetUniformLocation(m_ID, vName.c_str()), 1, GL_FALSE, &vMat[0][0]);
    }

    void CShader::__checkCompileErrors(GLuint vShader, std::string vType)
    {
        GLint Success;
        GLchar InfoLog[1024];
        if (vType != "PROGRAM")
        {
            glGetShaderiv(vShader, GL_COMPILE_STATUS, &Success);
            if (!Success)
            {
                glGetShaderInfoLog(vShader, 1024, NULL, InfoLog);
                std::cout << "ERROR::SHADER_COMPILATION_ERROR of vType: " << vType << "\n" << InfoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
        else
        {
            glGetProgramiv(vShader, GL_LINK_STATUS, &Success);
            if (!Success)
            {
                glGetProgramInfoLog(vShader, 1024, NULL, InfoLog);
                std::cout << "ERROR::PROGRAM_LINKING_ERROR of vType: " << vType << "\n" << InfoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
    }

}