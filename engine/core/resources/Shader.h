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
	// A lightweight wrapper for an actual shader in the Resource Manager.
	class ShaderID final
	{
	public:
		explicit ShaderID(std::size_t index);

		// Creates an invalid ShaderID.
		explicit ShaderID();

		std::size_t id() const;
		operator std::size_t() const;

		bool isValid() const;

	private:
		std::size_t m_id;
	};

	// Should not be used directly. Activate via Material instead.
	class Shader
	{
	public:
		virtual ~Shader() = 0;

		virtual void setBool(const std::string& name, bool value) const = 0;
		virtual void setInt(const std::string& name, std::int32_t value) const = 0;
		virtual void setFloat(const std::string& name, std::float_t value) const = 0;
		virtual void setVector2(const std::string& name, const Vector2& value) const = 0;
		virtual void setVector3(const std::string& name, const Vector3& value) const = 0;
		virtual void setVector4(const std::string& name, const Vector4& value) const = 0;
		virtual void setMatrix2x2(const std::string& name, const Matrix2x2& mat) const = 0;
		virtual void setMatrix3x3(const std::string& name, const Matrix3x3& mat) const = 0;
		virtual void setMatrix4x4(const std::string& name, const Matrix4x4& mat) const = 0;
	};
} // namespace engine