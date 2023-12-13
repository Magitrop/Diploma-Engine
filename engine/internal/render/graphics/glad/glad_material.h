#pragma once

#include <unordered_map>

#include <engine/core/resources/material.h>
#include <engine/core/resources/shader.h>

#include <engine/internal/render/graphics/glad/glad_graphic_api.h>
#include <engine/dependencies/gl/glad/include/glad/glad.h>

namespace engine
{
	class GladMaterial final : public Material
	{
		// friends
	private:
		friend class GladRenderPipeline;
		friend class GladResourceManager;

		// members
	public:
		explicit GladMaterial(ShaderID shader = ShaderID());
		virtual ~GladMaterial() override {}

		virtual void setBool(const std::string& name, bool value) const override;
		virtual void setInt(const std::string& name, GLint value) const override;
		virtual void setFloat(const std::string& name, GLfloat value) const override;
		virtual void setVector2(const std::string& name, const Vector2& value) const override;
		virtual void setVector3(const std::string& name, const Vector3& value) const override;
		virtual void setVector4(const std::string& name, const Vector4& value) const override;
		virtual void setMatrix2x2(const std::string& name, const Matrix2x2& mat) const override;
		virtual void setMatrix3x3(const std::string& name, const Matrix3x3& mat) const override;
		virtual void setMatrix4x4(const std::string& name, const Matrix4x4& mat) const override;

	private:
		ShaderID m_shader;
	};
} // namespace engine