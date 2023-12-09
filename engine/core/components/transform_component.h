#pragma once

#include <memory>

#include <engine/core/components/component_manager.h>
#include <engine/core/math/vector3.h>
#include <engine/core/math/quaternion.h>

namespace engine
{
	struct TransformRef final
	{
		Vector3& position;
		Quaternion& rotation;
		Vector3& scale;
	};

	class TransformComponent final : public ComponentManager
	{
	public:
		TransformComponent();
		virtual ~TransformComponent() override;

	public:
		TransformRef getTransform(ComponentID id);

	protected:
		virtual void attachComponent(ComponentID component) override;
		virtual void detachComponent(ComponentID component) override;

	private:
		class Internal;
		std::unique_ptr<Internal> m_internal;
	};
} // namespace engine