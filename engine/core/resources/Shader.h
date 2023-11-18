#pragma once

#include <engine/core/math/vector2.h>
#include <engine/core/math/vector3.h>
#include <engine/core/math/vector4.h>

#include <string>
#include <map>

namespace engine
{
    class Shader
    {
    public:
        Shader(const char* vertexPath = "", const char* fragmentPath = "");
        void use() const;
        void setBool(const std::string& name, bool value) const;
        void setInt(const std::string& name, int value) const;
        void setFloat(const std::string& name, float value) const;
        void setVector2(const std::string& name, const Vector2& value) const;
        void setVector2(const std::string& name, float x, float y) const;
        void setVector3(const std::string& name, const Vector3& value) const;
        void setVector3(const std::string& name, float x, float y, float z) const;
        void setVector4(const std::string& name, const Vector4& value) const;
        void setVector4(const std::string& name, float x, float y, float z, float w) const;
        void setMatrix2x2(const std::string& name, const Matrix2x2& mat) const;
        void setMatrix3x3(const std::string& name, const Matrix3x3& mat) const;
        void setMatrix4x4(const std::string& name, const Matrix4x4& mat) const;
    private:
        void checkCompileErrors(unsigned int shader, std::string type);
        GLuint m_id;
    };
} // namespace engine