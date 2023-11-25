#include "Shader.h"

#include <fstream>
#include <sstream>
#include <iostream>

namespace engine
{
    Shader::Shader(std::size_t id)
        : m_id(id)
    {}

    void Shader::setBool(const std::string& name, bool value) const
    {
    }
    void Shader::setInt(const std::string& name, int value) const
    {
    }
    void Shader::setFloat(const std::string& name, float value) const
    {
    }
    void Shader::setVector2(const std::string& name, const Vector2& value) const
    {
    }
    void Shader::setVector2(const std::string& name, float x, float y) const
    {
    }
    void Shader::setVector3(const std::string& name, const Vector3& value) const
    {
    }
    void Shader::setVector3(const std::string& name, float x, float y, float z) const
    {
    }
    void Shader::setVector4(const std::string& name, const Vector4& value) const
    {
    }
    void Shader::setVector4(const std::string& name, float x, float y, float z, float w) const
    {
    }
    void Shader::setMatrix2x2(const std::string& name, const Matrix2x2& mat) const
    {
    }
    void Shader::setMatrix3x3(const std::string& name, const Matrix3x3& mat) const
    {
    }
    void Shader::setMatrix4x4(const std::string& name, const Matrix4x4& mat) const
    {
    }
} // namespace engine