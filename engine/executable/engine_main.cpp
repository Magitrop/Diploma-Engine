#include "engine_main.h"

#include <bitset>
#include <iostream>

#include <engine/debug/memory/memory_guard.h>
#include <engine/executable/editor_runtime_pipeline.h>
#include <engine/executable/runtime.h>
#include <engine/core/entity/entity_manager.h>

#include <engine/dependencies/fmt/include/fmt/format.h>
#include <engine/internal/helpers/persistent_vector.h>

/*
class EntityID
{
public:
	explicit EntityID()
		: m_id(static_cast<std::size_t>(-1))
	{}

	explicit EntityID(std::size_t id)
		: m_id(id)
	{}

public:
	std::size_t getID() const { return m_id; }
	operator std::size_t() const { return m_id; }

private:
	std::size_t m_id;
};

class ComponentID
{
public:
	explicit ComponentID()
		: m_id(static_cast<std::size_t>(-1))
	{}

	explicit ComponentID(std::size_t id)
		: m_id(id)
	{}

public:
	std::size_t getID() const { return m_id; }
	operator std::size_t() const { return m_id; }

private:
	std::size_t m_id;
};

template<size_t Index>
struct idx
{
	constexpr static size_t getIndex()
	{
		return Index;
	}
};

class ComponentBase;
class ObjectsManager
{
public:
	template<typename Component>
	ComponentID getComponentID(EntityID obj)
	{
		const auto& ids = m_componentVectors[Component::getIndex()].ids;
		for (std::size_t i = 0; i < ids.size(); ++i)
			if (ids[i] == obj.getID())
				return ComponentID(i);
		return ComponentID(static_cast<std::size_t>(-1));
	}

	template<typename Component>
	ComponentID attachComponent(EntityID obj)
	{
		// already attached
		if (m_objectsWithComponents[obj].test(Component::getIndex()))
			return ComponentID(static_cast<std::size_t>(-1));

		auto& ids = m_componentVectors[Component::getIndex()].ids;

		ids.push_back(obj);
		m_objectsWithComponents[obj][Component::getIndex()] = 1;

		return ComponentID(ids.size() - 1);
	}

	template<typename Component>
	void detachComponent(EntityID obj)
	{
		// already detached
		if (!m_objectsWithComponents[obj].test(Component::getIndex()))
			return;

		auto& ids = m_componentVectors[Component::getIndex()].ids;

		*std::find(ids.begin(), ids.end(), obj) = EntityID(static_cast<size_t>(-1));
		//ids.erase(std::find(ids.begin(), ids.end(), obj));
		m_objectsWithComponents[obj][Component::getIndex()] = 0;
	}

	template<typename ComponentHandler>
	std::shared_ptr<ComponentHandler> getComponentHandler()
	{
		m_componentVectors[ComponentHandler::getIndex()].handler;
	}

	EntityID createObject()
	{
		m_objectsWithComponents.emplace_back();
		return EntityID(m_objectsWithComponents.size() - 1);
	}

	void destroyObject(EntityID obj)
	{
		for (auto& [compID, objects] : m_componentVectors)
		{
			auto& ids = objects.ids;
			//ids.erase(std::find(ids.begin(), ids.end(), obj));
			*std::find(ids.begin(), ids.end(), obj) = EntityID(static_cast<size_t>(-1));
		}
		*(m_objectsWithComponents.begin() + obj.getID()) = 0;
		//m_objectsWithComponents.erase(m_objectsWithComponents.begin() + obj.getID());
	}

	EntityID getObject(std::size_t index)
	{
		if (index > m_objectsWithComponents.size() - 1)
			return EntityID(static_cast<std::size_t>(-1));
		return EntityID(index);
	}

	template<typename... Components>
	std::vector<EntityID> getObjectsWithComponents()
	{
		// getting the max size of the resulting vector to prevent redundant allocations
		std::size_t maxSize = 0;
		[&maxSize, this]<typename... Components>(Components&&... args)
		{
			((maxSize = std::max(m_componentVectors[args].ids.size(), maxSize)), ...);
		}(Components::getIndex()...);

		if (maxSize == 0)
			return {};

		std::vector<EntityID> objects;
		objects.reserve(maxSize);
		for (std::size_t i = 0; i < m_objectsWithComponents.size(); ++i)
		{
			bool select = true;
			[&select, i, this]<typename... Components>(Components&&... args)
			{
				((select &= m_objectsWithComponents[i].test(args)), ...);
			}(Components::getIndex()...);

			if (select)
			{
				objects.push_back(getObject(i));
			}
		}
		objects.shrink_to_fit();
		return objects;
	}

private:
	struct ComponentVector
	{
		std::shared_ptr<ComponentBase> handler = nullptr;
		std::vector<EntityID> ids;
	};
	std::unordered_map<std::size_t, ComponentVector> m_componentVectors;
	/*

	components[ compId ] -> transforms[ objCompId ] -> objId

					0	 1	   2	 3	  4
	0 (Transform):  [87] [158] [279] [11] [515] ...
	1 (Render):		[87] [279] [515] ...
	...
	4 (Mesh):		[87] [515] ...
	...


	// TODO: provide an opportunity to (dynamically?) extend the capacity of the bitset
	// Extension (copying from a narrower one and pasting into a wider one) must be performed via the bitset stream operators << and >>
	// Also inform the user about that extension and require them to accept it - otherwise further creation of components must be forbidden
	std::vector<std::bitset<256>> m_objectsWithComponents;
	/*

	0	[00...010111]
	1	[00...010001]
	2	[00...110011]
	3	[00...111111]
	...

			   2^0=1=1			  2^1=2=10			2^4=16=10000 = 10011
	Transform (0)	   && Render (1)	   && Mesh (4)			 = [00...010011] => returns (0, 2, 3)

};

class ObjectsManagerGetter
{
public:
	ObjectsManager* objectsManager() { return m_manager; }
private:
	ObjectsManager* m_manager;
};

class CurrentObjectGetter
{
public:
	EntityID gameObject() { return m_gameObject; }
private:
	EntityID m_gameObject;
};

class ComponentBase : public ObjectsManagerGetter
{

};

class TransformComponent : public idx<0>, public ComponentBase
{
public:
	float& x(EntityID obj)
	{
		return m_x[objectsManager()->getComponentID<TransformComponent>(obj)];
	}
	float& y(EntityID obj)
	{
		return m_y[objectsManager()->getComponentID<TransformComponent>(obj)];
	}
	float& z(EntityID obj)
	{
		return m_z[objectsManager()->getComponentID<TransformComponent>(obj)];
	}

private:
	std::vector<float> m_x;
	std::vector<float> m_y;
	std::vector<float> m_z;
};

class CameraComponent : public idx<1>, public ComponentBase
{
	friend class ObjectsManager;

public:
	float& farPlane(EntityID obj)
	{
		return m_farPlanes[objectsManager()->getComponentID<CameraComponent>(obj)];
	}

	float& nearPlane(EntityID obj)
	{
		return m_nearPlanes[objectsManager()->getComponentID<CameraComponent>(obj)];
	}

	float& fov(EntityID obj)
	{
		return m_fovs[objectsManager()->getComponentID<CameraComponent>(obj)];
	}

private:
	std::vector<float> m_farPlanes;
	std::vector<float> m_nearPlanes;
	std::vector<float> m_fovs;
};

/*

COMPONENT_BEGIN(MyScript)
	COMPONENT_METHOD(onTick)
	void onTick() {}

	COMPONENT_FIELD(float, abc)
	COMPONENT_FIELD(std::vector<int>, myVec)

	COMPONENT_FLOAT(a)
	COMPONENT_BOOL(b)
	COMPONENT_STRING(c)
	COMPONENT_OBJECT(d)
COMPONENT_END()


// ...component unique ID assignment (via template magic)...
class MyScript final : public idx<5>, public ComponentBase, public CurrentObjectGetter
{
private:
	MyScript(MyScript&) = delete;
	MyScript(MyScript&&) = delete;
	MyScript& operator = (MyScript&) = delete;
	MyScript& operator = (MyScript&&) = delete;
public:
	void onCreated()
	{

	}

	void onTick()
	{

	}

private:
	std::vector<float> m_a;
public:
	float& a(EntityID obj) { return m_a[objectsManager()->getComponentID<MyScript>(obj)]; }

private:
	std::vector<char> m_b;
public:
	char& b(EntityID obj) { return m_b[objectsManager()->getComponentID<MyScript>(obj)]; }

private:
	std::vector<std::string> m_c;
public:
	std::string& c(EntityID obj) { return m_c[objectsManager()->getComponentID<MyScript>(obj)]; }

private:
	std::vector<EntityID> m_d;
public:
	EntityID& d(EntityID obj) { return m_d[objectsManager()->getComponentID<MyScript>(obj)]; }
};
*/

namespace engine
{
	int Main::run()
	{
		MEMORY_GUARD;

		Runtime runtime;
		runtime.setPipeline(new EditorRuntimePipeline());
		runtime.initialize();
		runtime.run();

		return 0;
	}
} // namespace engine
