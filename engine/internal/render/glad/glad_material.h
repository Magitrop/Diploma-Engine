#pragma once

#include <unordered_map>

#include <engine/core/resources/resource_manager.h>
#include <engine/internal/render/glad/glad_graphic_api.h>

namespace engine
{
	class GladShader;
	class GladMaterialImpl;
	class GladMaterialAccessor final : public MaterialAccessor 
	{
	public:
		explicit GladMaterialAccessor(GladMaterialImpl* impl);

		virtual void setBool(const std::string& name, bool value) override;
		virtual void setInt(const std::string& name, GLint value) override;
		virtual void setFloat(const std::string& name, GLfloat value) override;
		virtual void setVector2(const std::string& name, const Vector2& value) override;
		virtual void setVector3(const std::string& name, const Vector3& value) override;
		virtual void setVector4(const std::string& name, const Vector4& value) override;
		virtual void setMatrix2x2(const std::string& name, const Matrix2x2& value) override;
		virtual void setMatrix3x3(const std::string& name, const Matrix3x3& value) override;
		virtual void setMatrix4x4(const std::string& name, const Matrix4x4& value) override;

		virtual std::int8_t renderQueue() const override;

	private:
		GladMaterialImpl* m_impl;
	};

	class GladMaterialImpl final
	{
		// members
	public:
		GladMaterialImpl(GladShader* shader, std::int8_t renderQueue);
		// Creates an invalid material implementation.
		GladMaterialImpl();

		void setBool(const std::string& name, bool value);
		void setInt(const std::string& name, GLint value);
		void setFloat(const std::string& name, GLfloat value);
		void setVector2(const std::string& name, const Vector2& value);
		void setVector3(const std::string& name, const Vector3& value);
		void setVector4(const std::string& name, const Vector4& value);
		void setMatrix2x2(const std::string& name, const Matrix2x2& value);
		void setMatrix3x3(const std::string& name, const Matrix3x3& value);
		void setMatrix4x4(const std::string& name, const Matrix4x4& value);

		void useShader() const;

		std::int8_t renderQueue() const { return m_renderQueue; }
		const GladShader* shader() const { return m_shader; }

	private:
		GladShader* m_shader;
		std::int8_t m_renderQueue;
	};
} // namespace engine