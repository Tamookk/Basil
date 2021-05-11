/*
 * This header file declares the entity class, used for the ECS.
 */
#pragma once

#include "Scene/Scene.h"

#include <entt.hpp>

namespace Basil
{
	class Entity
	{
		public:
			Entity() = default;
			Entity(entt::entity handle, Scene* scene);
			Entity(const Entity& other) = default;
			
			template <typename T, typename ... Args>
			T& addComponent(Args&&... args)
			{
				ASSERT(!hasComponent<T>(), "Entity already has component.");
				T& component = scene->registry.emplace<T>(entity, std::forward<Args>(args)...);
				scene->onComponentAdded<T>(*this, component);
				return component;
			}

			template <typename T>
			T& getComponent()
			{
				ASSERT(hasComponent<T>(), "Entity does not have component.");
				return scene->registry.get<T>(entity);
			}

			template <typename T>
			bool hasComponent()
			{
				return scene->registry.all_of<T>(entity);
			}

			template <typename T>
			void removeComponent()
			{
				ASSERT(hasComponent<T>(), "Entity does not have component.");
				scene->registry.remove<T>(entity);
			}

			operator bool() const;
			operator entt::entity() const;
			operator uint32_t() const;
			bool operator==(const Entity& other) const;
			bool operator!=(const Entity& other) const;
		private:
			entt::entity entity = entt::null;
			Scene* scene = nullptr;
	};
}