#include "Shader.h"

#include <engine/dependencies/gl/glfw/include/GLFW/glfw3.h>

#include <fstream>
#include <sstream>
#include <iostream>

namespace engine
{
    Shader::Shader(const char* vertexPath, const char* fragmentPath)
    {
        GraphicAPI::createShader(vertexPath, fragmentPath);

        // 1. retrieve the vertex/fragment source code from filePath
        std::string vertexCode;
        std::string fragmentCode;
        std::ifstream vShaderFile(vertexPath);
        std::ifstream fShaderFile(fragmentPath);
        try
        {
            std::stringstream buffer1, buffer2;

            buffer1 << vShaderFile.rdbuf();
            vertexCode = buffer1.str();

            buffer2 << fShaderFile.rdbuf();
            fragmentCode = buffer2.str();

            vShaderFile.close();
            fShaderFile.close();
        }
        catch (std::ifstream::failure& e)
        {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: " << e.what() << std::endl;
            return;
        }
        const char* vShaderCode = vertexCode.c_str();
        const char* fShaderCode = fragmentCode.c_str();

        // 2. compile shaders
        unsigned int vertex, fragment;

        // vertex shader
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
        std::cout << "Now compiling " << vertexPath << std::endl;
        CheckCompileErrors(vertex, "VERTEX");

        // fragment Shader
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);
        std::cout << "Now compiling " << fragmentPath << std::endl;
        CheckCompileErrors(fragment, "FRAGMENT");

        // shader Program
        m_id = glCreateProgram();
        glAttachShader(m_id, vertex);
        glAttachShader(m_id, fragment);
        glLinkProgram(m_id);
        CheckCompileErrors(m_id, "PROGRAM");

        // delete the shaders as they're linked into our program now and no longer necessary
        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }

    void Shader::use() const
    {
        glUseProgram(m_id);
    }

    void Shader::setBool(const std::string& name, bool value) const
    {
        glUniform1i(glGetUniformLocation(m_id, name.c_str()), (int)value);
    }
    void Shader::setInt(const std::string& name, int value) const
    {
        glUniform1i(glGetUniformLocation(m_id, name.c_str()), value);
    }
    void Shader::setFloat(const std::string& name, float value) const
    {
        glUniform1f(glGetUniformLocation(m_id, name.c_str()), value);
    }
    void Shader::setVector2(const std::string& name, const Vector2& value) const
    {
        glUniform2fv(glGetUniformLocation(m_id, name.c_str()), 1, &value[0]);
    }
    void Shader::setVector2(const std::string& name, float x, float y) const
    {
        glUniform2f(glGetUniformLocation(m_id, name.c_str()), x, y);
    }
    void Shader::setVector3(const std::string& name, const Vector3& value) const
    {
        glUniform3fv(glGetUniformLocation(m_id, name.c_str()), 1, &value[0]);
    }
    void Shader::setVector3(const std::string& name, float x, float y, float z) const
    {
        glUniform3f(glGetUniformLocation(m_id, name.c_str()), x, y, z);
    }
    void Shader::setVector4(const std::string& name, const Vector4& value) const
    {
        glUniform4fv(glGetUniformLocation(m_id, name.c_str()), 1, &value[0]);
    }
    void Shader::setVector4(const std::string& name, float x, float y, float z, float w) const
    {
        glUniform4f(glGetUniformLocation(m_id, name.c_str()), x, y, z, w);
    }
    void Shader::setMatrix2x2(const std::string& name, const Matrix2x2& mat) const
    {
        glUniformMatrix2fv(glGetUniformLocation(m_id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    void Shader::setMatrix3x3(const std::string& name, const Matrix3x3& mat) const
    {
        glUniformMatrix3fv(glGetUniformLocation(m_id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    void Shader::setMatrix4x4(const std::string& name, const Matrix4x4& mat) const
    {
        glUniformMatrix4fv(glGetUniformLocation(m_id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

    void Shader::checkCompileErrors(unsigned int shader, std::string type)
    {
        int success;
        char infoLog[1024];
        if (type != "PROGRAM")
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
    }
} // namespace engine