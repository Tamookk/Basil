#include "pch.h"
#include "Scene/Entity.h"

namespace Basil
{
	// Constructor
	Entity::Entity(entt::entity handle, Scene* scene)
	{
		// Initialise variables
		entity = handle;
		this->scene = scene;
	}

	// Bool operator
	Entity::operator bool() const
	{
		return entity != entt::null;
	}
}