#include <iostream>

#include <engine/debug/memory/memory_guard.h>
#include <engine/executable/editor_runtime_pipeline.h>
#include <engine/executable/runtime.h>

#include <engine/dependencies/fmt/include/fmt/format.h>

using Runtime = engine::Runtime;
using EditorRuntimePipeline = engine::EditorRuntimePipeline;

template<size_t Index>
struct idx
{
	constexpr static size_t getIndex()
	{
		return Index;
	}
};

class ComponentManager
{
protected:
	virtual ~ComponentManager() = 0;
};
ComponentManager::~ComponentManager() {}

class Component
{
protected:
	Component(std::size_t index, ComponentManager* manager)
		: m_manager(manager)
		, m_index(index) 
	{}
	virtual ~Component() = 0;

	ComponentManager* m_manager;
	size_t m_index;
};
Component::~Component() {}

class CameraComponentManager : public ComponentManager
{
	friend class CameraComponent;
	friend class ObjectsManager;

private:
	std::vector<float> m_farPlanes;
	std::vector<float> m_nearPlanes;
	std::vector<float> m_fovs;
};

class CameraComponent : public idx<2>, public Component
{
public:
	CameraComponent(std::size_t index, ComponentManager* manager) 
		: Component(index, manager) 
	{}

	float& farPlane()
	{
		return static_cast<CameraComponentManager*>(m_manager)->m_farPlanes[m_index];
	}

	float& nearPlane()
	{
		return static_cast<CameraComponentManager*>(m_manager)->m_nearPlanes[m_index];
	}

	float& fov()
	{
		return static_cast<CameraComponentManager*>(m_manager)->m_fovs[m_index];
	}
};

class ObjectsManager
{
public:
	template<typename Comp>
	Comp getComponent(size_t obj)
	{
		const auto& vector = m_componentVectors[Comp::getIndex()].components;
		for (std::size_t i = 0; i < vector.size(); ++i)
			if (vector[i] == obj)
				return Comp(i, m_componentVectors[Comp::getIndex()].manager);
		return Comp(static_cast<std::size_t>(-1), nullptr);
	}

	template<typename Comp>
	Comp addComponent(std::size_t obj)
	{
		constexpr std::size_t compId = Comp::getIndex();

		auto& vector = m_componentVectors[compId].components;
		std::size_t newCompId = vector.size();
		vector.push_back(obj);

		CameraComponentManager* manager = static_cast<CameraComponentManager*>(m_componentVectors[compId].manager);
		manager->m_farPlanes.emplace_back();
		manager->m_fovs.emplace_back();
		manager->m_nearPlanes.emplace_back();

		return Comp(newCompId, m_componentVectors[compId].manager);
	}

	template<typename Comp>
	void addComponentManager(ComponentManager* manager)
	{
		m_componentVectors[Comp::getIndex()].manager = manager;
	}

private:
	struct ComponentVector
	{
		ComponentManager* manager;
		std::vector<std::size_t> components;
	};

	std::unordered_map<std::size_t, ComponentVector> m_componentVectors;

	/*

	components[ compId ] -> transforms[ objCompId ] -> objId

	0    1     2            16
	[87] [158] [279] [] ... [7]

	*/
};

int main()
{
	MEMORY_GUARD;

	constexpr std::size_t compId = CameraComponent::getIndex();
	ObjectsManager objManager;
	objManager.addComponentManager<CameraComponent>(new CameraComponentManager());

	objManager.addComponent<CameraComponent>(157);
	objManager.addComponent<CameraComponent>(259);
	objManager.addComponent<CameraComponent>(16);
	objManager.addComponent<CameraComponent>(9);
	objManager.addComponent<CameraComponent>(3);
	objManager.addComponent<CameraComponent>(171);

	{
		CameraComponent cam = objManager.getComponent<CameraComponent>(16);
		cam.farPlane() = 1.0f;
		cam.nearPlane() = 0.01f;
		cam.fov() = 60;
	}

	{
		CameraComponent cam = objManager.getComponent<CameraComponent>(171);
		cam.farPlane() = 5.0f;
		cam.nearPlane() = 0.01f;
		cam.fov() = 100;
	}

	Runtime runtime;
	runtime.setPipeline<EditorRuntimePipeline>();
	runtime.initialize();
	runtime.run();
}