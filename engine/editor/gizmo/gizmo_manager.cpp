#include "gizmo_manager.h"

#include <engine/core/components/transform_component.h>
#include <engine/core/entity/entity_manager.h>
#include <engine/core/input/input_system.h>
#include <engine/core/math/ray.h>
#include <engine/core/math/math_helpers.h>
#include <engine/core/resources/resource_manager.h>

#include <engine/editor/editor.h>
#include <engine/editor/gizmo/editor_drawer.h>
#include <engine/editor/gui/editor_viewport_window.h>

#include <engine/internal/helpers/assert.h>

namespace engine
{
	GizmoManager::GizmoManager(DependencyContext dependencies)
		: Dependencies(dependencies)
		, m_gizmo(new Gizmo())
	{}

	void GizmoManager::tick()
	{
		Editor* editor = getDependency<Editor>();
		Transform* transformManager = getDependency<EntityManager>()->getComponentManager<Transform>();
		InputSystem* inputSystem = getDependency<InputSystem>();
		const EditorViewportWindow* window = editor->viewportsManager()->focusedViewport();

		Vector2 mouseMotion = inputSystem->mouseMotion();

		if (!isGizmoDragging() ||
			window == nullptr ||
			mouseMotion.isAlmostZero() ||
			!m_gizmo->isAttached())
			return;

		ComponentID transformID = m_gizmo->attachedTo();
		Vector3 pos = transformManager->localPosition(transformID);
		Vector2 mouse = window->screenToContent(inputSystem->mousePosition());
		Ray ray = window->viewport().screenPointToRay(mouse + mouseMotion);

		Vector3 forward = transformManager->forward(transformID);
		Vector3 right = transformManager->right(transformID);
		Vector3 up = transformManager->up(transformID);

		auto getAxisTranslation =
			[window, ray, pos, transformID, transformManager, this](Vector3 direction)
			{
				Vector3 planeIntersection = math::rayPlaneIntersection(ray, pos, m_planeNormal);
				Vector3 axis = planeIntersection - m_lastWorldPoint;
				m_lastWorldPoint = planeIntersection;
				transformManager->translateLocalBy(transformID, direction * Vector3::dot(axis, direction));
			};

		switch (m_gizmoDraggingMode)
		{
		case GizmoType::TranslateX:
		{
			// TODO: support scene graph transform hierarchy
			getAxisTranslation(right);
			break;
		}
		case GizmoType::TranslateY:
		{
			// TODO: support scene graph transform hierarchy
			getAxisTranslation(up);
			break;
		}
		case GizmoType::TranslateZ:
		{
			// TODO: support scene graph transform hierarchy
			getAxisTranslation(forward);
			break;
		}
		}
	}

	const Gizmo* GizmoManager::gizmo() const
	{
		return m_gizmo.get();
	}

	void GizmoManager::setGizmoType(GizmoType type)
	{
		m_gizmo->setGizmoType(type);
	}

	void GizmoManager::beginGizmoDragging(GizmoType type)
	{
		Editor* editor = getDependency<Editor>();
		const EditorViewportWindow* viewportWindow = editor->viewportsManager()->focusedViewport();
		Transform* transformManager = getDependency<EntityManager>()->getComponentManager<Transform>();
		InputSystem* inputSystem = getDependency<InputSystem>();

		ComponentID transformID = m_gizmo->attachedTo();
		Vector2 mouse = viewportWindow->screenToContent(inputSystem->mousePosition());
		Vector3 pos = transformManager->localPosition(transformID);

		Vector3 forward = transformManager->forward(transformID);
		Vector3 right = transformManager->right(transformID);
		Vector3 up = transformManager->up(transformID);

		Ray ray = viewportWindow->viewport().screenPointToRay(mouse);
		Vector3 bestNormal = Vector3::zero();

		// Get the best normal for an axis translation according to the camera position.
		// This makes the 2D view easier to be set.
		auto getBestNormalForAxis =
			[ray](Vector3 a, Vector3 b)
			{
				float aNormal = std::fabs(Vector3::dot(ray.direction, a));
				float bNormal = std::fabs(Vector3::dot(ray.direction, b));
				return aNormal > bNormal ? a : b;
			};

		switch (type)
		{
		case GizmoType::TranslateX:
		{
			bestNormal = getBestNormalForAxis(forward, up);
			break;
		}
		case GizmoType::TranslateY:
		{
			bestNormal = getBestNormalForAxis(right, forward);
			break;
		}
		case GizmoType::TranslateZ:
		{
			bestNormal = getBestNormalForAxis(right, up);
			break;
		}
		}

		m_lastWorldPoint = math::rayPlaneIntersection(ray, pos, bestNormal);
		m_planeNormal = bestNormal;

		m_gizmoDraggingMode = type;
		m_onBeginGizmoDragging(type);
	}

	void GizmoManager::endGizmoDragging()
	{
		m_gizmoDraggingMode = GizmoType::None;
		m_onEndGizmoDragging();

		//m_lastWorldPoint = 
		// TODO: connect the undo/redo
	}

	Matrix4x4 GizmoManager::getAttachedMatrix() const
	{
		const Transform* transformManager = getDependency<EntityManager>()->getComponentManager<Transform>();
		if (m_gizmo->isAttached())
			return transformManager->modelMatrix(m_gizmo->attachedTo());
		ERROR_LOG("Cannot get attached matrix: the gizmo does not have one!");
		return Matrix4x4();
	}

	bool GizmoManager::isAttached() const
	{
		return m_gizmo->isAttached();
	}

	bool GizmoManager::isGizmoDragging() const
	{
		return m_gizmoDraggingMode != GizmoType::None;
	}

	void GizmoManager::attach(ComponentID to)
	{
		if (to == ComponentID())
			return;

		m_gizmo->attach(to);
	}

	void GizmoManager::detach()
	{
		m_gizmo->detach();
	}
} // namespace engine