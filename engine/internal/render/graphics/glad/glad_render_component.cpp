#include "glad_render_component.h"

namespace engine
{
	void GladRenderComponent::attachComponent(ComponentID component)
	{
		GLuint VAO, VBO;
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		m_VAO.push(VAO);
		m_VBO.push(VBO);
	}

	void GladRenderComponent::detachComponent(ComponentID component)
	{
		glDeleteBuffers(1, &m_VBO.at(component)->get());
		glDeleteVertexArrays(1, &m_VAO.at(component)->get());

		m_VBO.remove(m_VBO.begin() + component);
		m_VAO.remove(m_VAO.begin() + component);
	}
} // namespace engine