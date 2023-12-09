#pragma once

#include <engine/core/resources/Shader.h>
#include <engine/internal/render/graphics/glad/glad_graphic_api.h>

namespace engine
{
	class GladShader final : public Shader
	{
	public:
		void setBool(const std::string& name, bool value) const override;
		void setInt(const std::string& name, int value) const override;
		void setFloat(const std::string& name, float value) const override;
		void setVector2(const std::string& name, const Vector2& value) const override;
		void setVector2(const std::string& name, float x, float y) const override;
		void setVector3(const std::string& name, const Vector3& value) const override;
		void setVector3(const std::string& name, float x, float y, float z) const override;
		void setVector4(const std::string& name, const Vector4& value) const override;
		void setVector4(const std::string& name, float x, float y, float z, float w) const override;
		void setMatrix2x2(const std::string& name, const Matrix2x2& mat) const override;
		void setMatrix3x3(const std::string& name, const Matrix3x3& mat) const override;
		void setMatrix4x4(const std::string& name, const Matrix4x4& mat) const override;

	private:

	};
} // namespace engine