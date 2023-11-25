#pragma once

#include <engine/core/math/vector2.h>
#include <engine/core/math/vector3.h>
#include <engine/core/math/vector4.h>
#include <engine/core/math/matrix2x2.h>
#include <engine/core/math/matrix3x3.h>
#include <engine/core/math/matrix4x4.h>

#include <string>
#include <map>

namespace engine
{
    // A lightweight wrapper for an actual shader in the Resource Manager.
    // Cannot be used directly. Activate via Material instead.
    class Shader final
    {
    private:
        Shader(std::size_t id);

    public:
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
        std::size_t m_id; // the ID of an actual shader
    };
} // namespace engine