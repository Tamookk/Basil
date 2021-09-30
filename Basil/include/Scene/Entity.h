/*
 * This header file declares the entity class, used for the ECS.
 */
#pragma once

#include "Core/UUID.h"
#include "Scene/Component.h"
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
			
			// Add a component
			template <typename T, typename ... Args>
			T& addComponent(Args&&... args)
			{
				ASSERT(!hasComponent<T>(), "Entity already has component.");
				T& component = scene->registry.emplace<T>(entity, std::forward<Args>(args)...);
				scene->onComponentAdded<T>(*this, component);
				return component;
			}

			// Add or replace a component
			template<typename T, typename... Args>
			T& addOrReplaceComponent(Args&&... args)
			{
				T& component = scene->registry.emplace_or_replace<T>(entity, std::forward<Args>(args)...);
				scene->onComponentAdded<T>(*this, component);
				return component;
			}

			// Get a component
			template <typename T>
			T& getComponent()
			{
				ASSERT(hasComponent<T>(), "Entity does not have component.");
				return scene->registry.get<T>(entity);
			}

			// Return whether an entity has a component
			template <typename T>
			bool hasComponent()
			{
				return scene->registry.all_of<T>(entity);
			}

			// Remove a component
			template <typename T>
			void removeComponent()
			{
				ASSERT(hasComponent<T>(), "Entity does not have component.");
				scene->registry.remove<T>(entity);
			}

			operator bool() const;
			operator entt::entity() const;
			operator uint32_t() const;
			UUID getUUID();
			const std::string& getName();
			bool operator==(const Entity& other) const;
			bool operator!=(const Entity& other) const;
		private:
			entt::entity entity = entt::null;
			Scene* scene = nullptr;
	};
}