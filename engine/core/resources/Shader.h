#pragma once

#include <map>
#include <memory>
#include <string>

#include <engine/core/math/vector2.h>
#include <engine/core/math/vector3.h>
#include <engine/core/math/vector4.h>
#include <engine/core/math/matrix2x2.h>
#include <engine/core/math/matrix3x3.h>
#include <engine/core/math/matrix4x4.h>

namespace engine
{
	// A lightweight abstract wrapper for an actual shader in the Resource Manager.
	// Cannot be used directly. Activate via Material instead.
	class Shader
	{
	protected:
		Shader(std::size_t id);

	public:
		virtual void setBool(const std::string& name, bool value) const = 0;
		virtual void setInt(const std::string& name, int value) const = 0;
		virtual void setFloat(const std::string& name, float value) const = 0;
		virtual void setVector2(const std::string& name, const Vector2& value) const = 0;
		virtual void setVector2(const std::string& name, float x, float y) const = 0;
		virtual void setVector3(const std::string& name, const Vector3& value) const = 0;
		virtual void setVector3(const std::string& name, float x, float y, float z) const = 0;
		virtual void setVector4(const std::string& name, const Vector4& value) const = 0;
		virtual void setVector4(const std::string& name, float x, float y, float z, float w) const = 0;
		virtual void setMatrix2x2(const std::string& name, const Matrix2x2& mat) const = 0;
		virtual void setMatrix3x3(const std::string& name, const Matrix3x3& mat) const = 0;
		virtual void setMatrix4x4(const std::string& name, const Matrix4x4& mat) const = 0;

	private:
		std::size_t m_id; // The ID of an actual shader
	};
} // namespace engine