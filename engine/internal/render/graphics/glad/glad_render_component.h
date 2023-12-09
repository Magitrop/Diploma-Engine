#pragma once

#include <memory>

#include <engine/dependencies/gl/glad/include/glad/glad.h>

#include <engine/core/components/render_component.h>

#include <engine/internal/core/constants/runtime_constants.h>
#include <engine/internal/helpers/persistent_vector.h>

namespace engine
{
	class GladRenderComponent final : public RenderComponent
	{
		// friends
	private:
		friend class GladRenderPipeline;

		// members
	private:
		GladRenderComponent() = default;
		virtual ~GladRenderComponent() override {};

	protected:
		virtual void attachComponent(ComponentID component) override;
		virtual void detachComponent(ComponentID component) override;

	private:
		PersistentVector<GLuint, constants::VERY_FREQUENT_COMPONENT> m_VAO; // Vertex array object.
		PersistentVector<GLuint, constants::VERY_FREQUENT_COMPONENT> m_VBO; // Vertex buffer object.
	};
} // namespace engine
