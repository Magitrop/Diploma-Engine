#include "glad_material.h"

namespace engine
{
	GladMaterial::GladMaterial(ShaderID shader /* = ShaderID() */)
		: m_shader(shader)
	{}

	void GladMaterial::setBool(const std::string& name, bool value) const
	{
	}
	void GladMaterial::setInt(const std::string& name, GLint value) const
	{
	}
	void GladMaterial::setFloat(const std::string& name, GLfloat value) const
	{
	}
	void GladMaterial::setVector2(const std::string& name, const Vector2& value) const
	{
	}
	void GladMaterial::setVector3(const std::string& name, const Vector3& value) const
	{
	}
	void GladMaterial::setVector4(const std::string& name, const Vector4& value) const
	{
	}
	void GladMaterial::setMatrix2x2(const std::string& name, const Matrix2x2& mat) const
	{
	}
	void GladMaterial::setMatrix3x3(const std::string& name, const Matrix3x3& mat) const
	{
	}
	void GladMaterial::setMatrix4x4(const std::string& name, const Matrix4x4& mat) const
	{
	}
} // namespace engine