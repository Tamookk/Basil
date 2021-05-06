/*
 * This header file declares a class for game scenes.
 */
#pragma once

#include "Core/Timestep.h"

#include <entt.hpp>

namespace Basil
{
	class Scene
	{
		public:
			Scene();
			~Scene();
			entt::entity createEntity();
			entt::registry& reg();
			void onUpdate(Timestep timeStep);
		private:
			entt::registry registry;
	};
}