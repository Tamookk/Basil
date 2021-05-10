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
		// Initialis variables
		viewportWidth = 0;
		viewportHeight = 0;
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
		// Update scripts
		{
			registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc)
			{
				if (!nsc.instance)
				{
					nsc.instance = nsc.instantiateScript();
					nsc.instance->entity = Entity{ entity, this };
					nsc.instance->onCreate();
				}
					
				nsc.instance->onUpdate(timeStep);
			});
		}

		// Render 2D
		Camera* mainCamera = nullptr;
		glm::mat4* cameraTransform = nullptr;
		{
			auto view = registry.view<TransformComponent, CameraComponent>();
			for (auto entity : view)
			{
				auto [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);
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
			Renderer2D::beginScene(*mainCamera, *cameraTransform);
			auto group = registry.group<TransformComponent>(entt::get<SpriteRenderComponent>);
			for (auto entity : group)
			{
				auto [transform, sprite] = group.get<TransformComponent, SpriteRenderComponent>(entity);
				Renderer2D::drawQuad(transform, sprite.color);
			}
			Renderer2D::endScene();
		}
	}

	// On viewport resize
	void Scene::onViewportResize(uint32_t width, uint32_t height)
	{
		// Update class variables
		viewportWidth = width;
		viewportHeight = height;

		// Resize camera
		auto view = registry.view<CameraComponent>();
		for (auto entity : view)
		{
			auto& cameraComponent = view.get<CameraComponent>(entity);
			if (!cameraComponent.fixedAspectRatio)
				cameraComponent.camera.setViewportSize(width, height);
		}
	}
}