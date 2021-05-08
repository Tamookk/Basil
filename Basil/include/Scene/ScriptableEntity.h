/*
 * This header declares a class for an entity that can be scripted.
 */
#pragma once

#include "Scene/Entity.h"

namespace Basil
{
	class ScriptableEntity
	{
		public:
			template <typename T>
			T& getComponent()
			{
				return entity.getComponent<T>();
			}
		private:
			Entity entity;
			friend class Scene;
	};
}