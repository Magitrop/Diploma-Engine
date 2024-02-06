#include "glad_material.h"

#include <engine/debug/logging/debug_logger.h>
#include <engine/internal/helpers/assert.h>
#include <engine/internal/render/glad/glad_shader.h>

namespace engine
{
	GladMaterialAccessor::GladMaterialAccessor(GladMaterialImpl* impl)
	{
		DEBUG_ASSERT(impl != nullptr);

		m_impl = impl;
	}

	void GladMaterialAccessor::setBool(const std::string& name, bool value)
	{
		m_impl->setBool(name, value);
	}
	void GladMaterialAccessor::setInt(const std::string& name, GLint value)
	{
		m_impl->setInt(name, value);
	}
	void GladMaterialAccessor::setFloat(const std::string& name, GLfloat value)
	{
		m_impl->setFloat(name, value);
	}
	void GladMaterialAccessor::setVector2(const std::string& name, const Vector2& value)
	{
		m_impl->setVector2(name, value);
	}
	void GladMaterialAccessor::setVector3(const std::string& name, const Vector3& value)
	{
		m_impl->setVector3(name, value);
	}
	void GladMaterialAccessor::setVector4(const std::string& name, const Vector4& value)
	{
		m_impl->setVector4(name, value);
	}
	void GladMaterialAccessor::setMatrix2x2(const std::string& name, const Matrix2x2& value)
	{
		m_impl->setMatrix2x2(name, value);
	}
	void GladMaterialAccessor::setMatrix3x3(const std::string& name, const Matrix3x3& value)
	{
		m_impl->setMatrix3x3(name, value);
	}
	void GladMaterialAccessor::setMatrix4x4(const std::string& name, const Matrix4x4& value)
	{
		m_impl->setMatrix4x4(name, value);
	}

	std::int8_t GladMaterialAccessor::renderQueue() const
	{
		return m_impl->renderQueue();
	}

	GladMaterialImpl::GladMaterialImpl(GladShader* shader, std::int8_t renderQueue)
		: m_shader(shader)
		, m_renderQueue(renderQueue)
	{
		DEBUG_ASSERT(m_shader != nullptr);
	}

	GladMaterialImpl::GladMaterialImpl()
		: m_shader(nullptr)
		, m_renderQueue(0)
	{}

	void GladMaterialImpl::setBool(const std::string& name, bool value)
	{
	}
	void GladMaterialImpl::setInt(const std::string& name, GLint value)
	{
	}
	void GladMaterialImpl::setFloat(const std::string& name, GLfloat value)
	{
	}
	void GladMaterialImpl::setVector2(const std::string& name, const Vector2& value)
	{
	}
	void GladMaterialImpl::setVector3(const std::string& name, const Vector3& value)
	{
	}
	void GladMaterialImpl::setVector4(const std::string& name, const Vector4& value)
	{
	}
	void GladMaterialImpl::setMatrix2x2(const std::string& name, const Matrix2x2& value)
	{
	}
	void GladMaterialImpl::setMatrix3x3(const std::string& name, const Matrix3x3& value)
	{
	}
	void GladMaterialImpl::setMatrix4x4(const std::string& name, const Matrix4x4& value)
	{
	}
	void GladMaterialImpl::useShader() const
	{
		glUseProgram(m_shader->getProgramID());
	}
} // namespace engine