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

	// uint32_t cast operator
	Entity::operator uint32_t() const
	{
		return (uint32_t)entity;
	}

	// Equality operator
	bool Entity::operator==(const Entity& other) const
	{
		return entity == other.entity && scene == other.scene;
	}

	// Not equality operator
	bool Entity::operator!=(const Entity& other) const
	{
		return !(*this == other);
	}
}