#pragma once

#include <engine/core/resources/shader.h>
#include <engine/internal/render/glad/glad_graphic_api.h>

namespace engine
{
	class GladShader final : public Shader
	{
		// friends
	private:
		friend class GladResourceManager;

		// members
	public:
		void setBool(const std::string& name, bool value) const override;
		void setInt(const std::string& name, std::int32_t value) const override;
		void setFloat(const std::string& name, std::float_t value) const override;
		void setVector2(const std::string& name, const Vector2& value) const override;
		void setVector3(const std::string& name, const Vector3& value) const override;
		void setVector4(const std::string& name, const Vector4& value) const override;
		void setMatrix2x2(const std::string& name, const Matrix2x2& mat) const override;
		void setMatrix3x3(const std::string& name, const Matrix3x3& mat) const override;
		void setMatrix4x4(const std::string& name, const Matrix4x4& mat) const override;

		GLuint getProgramID() const;

	private:
		// Returns false if the there were errors during the shader initialization.
		bool setupShader(const std::string& shaderName, const GLchar* vertexCode, const GLchar* fragmentCode);

		GLuint m_programID = 0;
	};
} // namespace engine