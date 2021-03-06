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

	// entt::entity operator
	Entity::operator entt::entity() const
	{
		return entity;
	}

	// uint32_t cast operator
	Entity::operator uint32_t() const
	{
		return (uint32_t)entity;
	}

	// Get the UUID of a component
	UUID Entity::getUUID()
	{
		return getComponent<IDComponent>().id;
	}

	// Get the name of the entity
	const std::string& Entity::getName()
	{
		return getComponent<TagComponent>().tag;
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