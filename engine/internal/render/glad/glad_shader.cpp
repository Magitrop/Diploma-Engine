#include "glad_shader.h"

#include <engine/debug/logging/debug_logger.h>
#include <engine/debug/memory/memory_guard.h>

namespace engine
{
    void GladShader::setBool(const std::string& name, bool value) const
    {
        glUniform1i(glGetUniformLocation(m_programID, name.c_str()), (int)value);
    }
    void GladShader::setInt(const std::string& name, std::int32_t value) const
    {
        glUniform1i(glGetUniformLocation(m_programID, name.c_str()), value);
    }
    void GladShader::setFloat(const std::string& name, std::float_t value) const
    {
        glUniform1f(glGetUniformLocation(m_programID, name.c_str()), value);
    }
    void GladShader::setVector2(const std::string& name, const Vector2& value) const
    {
        glUniform2fv(glGetUniformLocation(m_programID, name.c_str()), 1, &value[0]);
    }
    void GladShader::setVector3(const std::string& name, const Vector3& value) const
    {
        glUniform3fv(glGetUniformLocation(m_programID, name.c_str()), 1, &value[0]);
    }
    void GladShader::setVector4(const std::string& name, const Vector4& value) const
    {
        glUniform4fv(glGetUniformLocation(m_programID, name.c_str()), 1, &value[0]);
    }
    void GladShader::setMatrix2x2(const std::string& name, const Matrix2x2& mat) const
    {
        glUniformMatrix2fv(glGetUniformLocation(m_programID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    void GladShader::setMatrix3x3(const std::string& name, const Matrix3x3& mat) const
    {
        glUniformMatrix3fv(glGetUniformLocation(m_programID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    void GladShader::setMatrix4x4(const std::string& name, const Matrix4x4& mat) const
    {
        glUniformMatrix4fv(glGetUniformLocation(m_programID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

    GLuint GladShader::getProgramID() const
    {
        return m_programID;
    }

    bool GladShader::setupShader(const std::string& shaderName, const GLchar* vertexCode, const GLchar* fragmentCode)
    {
        MEMORY_GUARD;

        auto checkErrors = [shaderName](GLuint id, const std::string& stage) -> bool
        {
            int success;
            char infoLog[1024];
            if (stage == "linking")
            {
                glGetProgramiv(id, GL_LINK_STATUS, &success);
                if (!success)
                {
                    glGetProgramInfoLog(id, 1024, NULL, infoLog);
                    ERROR_LOG("An error occured while linking the shader '{}': {}", shaderName, infoLog);
                }
            }
            else
            {
                glGetShaderiv(id, GL_COMPILE_STATUS, &success);
                if (!success)
                {
                    glGetShaderInfoLog(id, 1024, NULL, infoLog);
                    ERROR_LOG("An error occured while compiling the {} shader '{}': {}", stage, shaderName, infoLog);
                }
            }
            return !!success;
        };

        GLuint vertex, fragment;
        bool isOk = true;

        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vertexCode, NULL);
        glCompileShader(vertex);
        isOk &= checkErrors(vertex, "vertex");

        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fragmentCode, NULL);
        glCompileShader(fragment);
        isOk &= checkErrors(fragment, "fragment");

        m_programID = glCreateProgram();
        glAttachShader(m_programID, vertex);
        glAttachShader(m_programID, fragment);
        glLinkProgram(m_programID);
        isOk &= checkErrors(m_programID, "linking");

        if (isOk)
        {
            glDetachShader(m_programID, vertex);
            glDetachShader(m_programID, fragment);
        }
        else
        {
            // delete the shaders as they are linked into the program now and no longer necessary
            glDeleteShader(vertex);
            glDeleteShader(fragment);
            glDeleteProgram(m_programID);
        }

        return isOk;
    }
} // namespace engine