/*
 * This header file declares a class for game scenes.
 */
#pragma once

#include "Core/Timestep.h"

#include <entt.hpp>

namespace Basil
{
	class Entity;

	class Scene
	{
		public:
			Scene();
			~Scene();
			Entity createEntity(const std::string& name = std::string());
			void destroyEntity(Entity entity);
			entt::registry& reg();
			void onUpdate(Timestep timeStep);
			void onViewportResize(uint32_t width, uint32_t height);
			Entity getPrimaryCameraEntity();
		private:
			template <typename T>
			void onComponentAdded(Entity entity, T& component);

			entt::registry registry;
			uint32_t viewportWidth;
			uint32_t viewportHeight;
			friend class Entity;
			friend class PropertiesPanel;
			friend class SceneHierarchyPanel;
			friend class SceneSerializer;
	};
}