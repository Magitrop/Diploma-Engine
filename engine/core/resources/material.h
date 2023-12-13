#pragma once

#include <cstddef>
#include <string>

#include <engine/core/math/vector2.h>
#include <engine/core/math/vector3.h>
#include <engine/core/math/vector4.h>
#include <engine/core/math/matrix2x2.h>
#include <engine/core/math/matrix3x3.h>
#include <engine/core/math/matrix4x4.h>

namespace engine
{
	// A lightweight wrapper for an actual material in the Resource Manager.
	class MaterialID final
	{
		// members
	public:
		explicit MaterialID(std::size_t index);

		// Creates an invalid MaterialID.
		explicit MaterialID();

		std::size_t id() const;
		operator std::size_t() const;

		bool isValid() const;

	private:
		std::size_t m_id;
	};

	// Use to preset shader uniform variables.
	class Material
	{
	public:
		virtual ~Material() = 0;

		virtual void setBool(const std::string& name, bool value) const {};
		virtual void setInt(const std::string& name, int value) const {};
		virtual void setFloat(const std::string& name, float value) const {};
		virtual void setVector2(const std::string& name, const Vector2& value) const {};
		virtual void setVector3(const std::string& name, const Vector3& value) const {};
		virtual void setVector4(const std::string& name, const Vector4& value) const {};
		virtual void setMatrix2x2(const std::string& name, const Matrix2x2& mat) const {};
		virtual void setMatrix3x3(const std::string& name, const Matrix3x3& mat) const {};
		virtual void setMatrix4x4(const std::string& name, const Matrix4x4& mat) const {};
	};
} // namespace engine