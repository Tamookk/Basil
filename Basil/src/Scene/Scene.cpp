#include "pch.h"
#include "Renderer/Renderer2D.h"
#include "Scene/Component.h"
#include "Scene/Entity.h"

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
	Entity Scene::createEntity(const std::string& name)
	{
		// Create an entity
		Entity entity = { registry.create(), this };

		// Add a transform and tag component
		entity.addComponent<TransformComponent>();
		auto& tag = entity.addComponent<TagComponent>();
		tag.tag = name.empty() ? "Entity" : name;

		// Return the entity
		return entity;
	}

	// Return the registry
	entt::registry& Scene::reg()
	{
		return registry;
	}

	// On update function
	void Scene::onUpdate(Timestep timeStep)
	{
		Camera* mainCamera = nullptr;
		glm::mat4* cameraTransform = nullptr;
		{
			auto group = registry.view<TransformComponent, CameraComponent>();
			for (auto entity : group)
			{
				auto& [transform, camera] = group.get<TransformComponent, CameraComponent>(entity);
				if (camera.primary)
				{
					mainCamera = &camera.camera;
					transform.updateTransform();
					cameraTransform = &transform.transform;
					break;
				}
			}
		}

		if (mainCamera)
		{
			Renderer2D::beginScene(mainCamera->getProjection(), *cameraTransform);
			auto group = registry.group<TransformComponent>(entt::get<SpriteRenderComponent>);
			for (auto entity : group)
			{
				auto& [transform, sprite] = group.get<TransformComponent, SpriteRenderComponent>(entity);
				Renderer2D::drawQuad(transform, sprite.color);
			}
			Renderer2D::endScene();
		}
	}
}