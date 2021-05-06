#include "pch.h"
#include "Renderer/Renderer2D.h"
#include "Scene/Component.h"
#include "Scene/Scene.h"

#include <glm/glm.hpp>

namespace Basil
{
	// Constructor
	Scene::Scene()
	{
#if ENTT_EXAMPLE_CODE
		entt::entity entity = registry.create();
		registry.emplace<TransformComponent>(entity, glm::mat4(1.0f));
		registry.on_construct<TransformComponent>().connect<&OnTransformConstruct>();

		if (registry.has<TransformComponent>(entity))
			TransformComponent& transform = registry.get<TransformComponent>(entity);

		auto view = registry.view<TransformComponent>();
		for (auto entity : view)
		{
			TransformComponent& transform = view.get<TransformComponent>(entity);
		}

		auto group = registry.group<TransformComponent>(entt::get<MeshComponent>);
		for (auto entity : group)
		{
			auto& [transform, mesh] = group.get<TransformComponent, MeshComponent>(entity);
		}
#endif
	}

	// Destructor
	Scene::~Scene()
	{

	}

	// Create an entity
	entt::entity Scene::createEntity()
	{
		return registry.create();
	}

	// Return the registry
	entt::registry& Scene::reg()
	{
		return registry;
	}

	// On update function
	void Scene::onUpdate(Timestep timeStep)
	{
		auto group = registry.group<TransformComponent>(entt::get<SpriteRenderComponent>);
		for (auto entity : group)
		{
			auto& [transform, sprite] = group.get<TransformComponent, SpriteRenderComponent>(entity);
			Renderer2D::drawQuad(transform, sprite.color);
		}
	}
}