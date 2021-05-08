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
			virtual ~ScriptableEntity() {}

			template <typename T>
			T& getComponent()
			{
				return entity.getComponent<T>();
			}
		protected:
			virtual void onCreate() {}
			virtual void onDestroy() {}
			virtual void onUpdate(Timestep timeStep) {}
		private:
			Entity entity;
			friend class Scene;
	};
}