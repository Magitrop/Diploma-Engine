#include "glad_shader.h"

namespace engine
{
    void GladShader::setBool(const std::string& name, bool value) const
    {
    }
    void GladShader::setInt(const std::string& name, std::int32_t value) const
    {
    }
    void GladShader::setFloat(const std::string& name, std::float_t value) const
    {
    }
    void GladShader::setVector2(const std::string& name, const Vector2& value) const
    {
    }
    void GladShader::setVector3(const std::string& name, const Vector3& value) const
    {
    }
    void GladShader::setVector4(const std::string& name, const Vector4& value) const
    {
    }
    void GladShader::setMatrix2x2(const std::string& name, const Matrix2x2& mat) const
    {
    }
    void GladShader::setMatrix3x3(const std::string& name, const Matrix3x3& mat) const
    {
    }
    void GladShader::setMatrix4x4(const std::string& name, const Matrix4x4& mat) const
    {
    }

    bool GladShader::setupShader(const std::string& shaderName, const GLchar* vertexCode, const GLchar* fragmentCode) const
    {
        return true;
    }
} // namespace engine