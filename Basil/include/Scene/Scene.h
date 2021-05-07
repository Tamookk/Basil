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
			entt::registry& reg();
			void onUpdate(Timestep timeStep);
		private:
			entt::registry registry;
			friend class Entity;
	};
}