/*
 * This header file declares a class for game scenes.
 */
#pragma once

#include "Core/Timestep.h"
#include "Core/UUID.h"
#include "Renderer/EditorCamera.h"

#include <entt.hpp>

class b2World;

namespace Basil
{
	class Entity;

	class Scene
	{
		public:
			Scene();
			~Scene();
			static Shared<Scene> copy(Shared<Scene> other);
			
			Entity createEntity(const std::string& name = std::string());
			Entity createEntityWithUUID(UUID uuid, const std::string& name = std::string());
			void destroyEntity(Entity entity);
			
			void onRuntimeStart();
			void onRuntimeStop();
			
			entt::registry& reg();
			
			void onUpdateRuntime(Timestep timeStep);
			void onUpdateEditor(Timestep timeStep, EditorCamera& camera);
			void onViewportResize(uint32_t width, uint32_t height);
			
			void duplicateEntity(Entity entity);
			
			Entity getPrimaryCameraEntity();

			template<typename... Components>
			auto getAllEntitiesWith()
			{
				return registry.view<Components...>();
			}

		private:
			template <typename T>
			void onComponentAdded(Entity entity, T& component);

			entt::registry registry;
			uint32_t viewportWidth;
			uint32_t viewportHeight;

			b2World* box2DWorld;

			friend class Entity;
			friend class PropertiesPanel;
			friend class SceneHierarchyPanel;
			friend class SceneSerializer;
	};
}