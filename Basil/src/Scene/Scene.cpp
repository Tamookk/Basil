#include "pch.h"
#include "Renderer/Renderer2D.h"
#include "Scene/Component.h"
#include "Scene/Entity.h"
#include "Scene/Scene.h"

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

	// Destroy an entity
	void Scene::destroyEntity(Entity entity)
	{
		registry.destroy(entity);
	}

	// Return the registry
	entt::registry& Scene::reg()
	{
		return registry;
	}

	// On update function for runtime
	void Scene::onUpdateRuntime(Timestep timeStep)
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
		glm::mat4 cameraTransform;
		{
			auto view = registry.view<TransformComponent, CameraComponent>();
			for (auto entity : view)
			{
				auto [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);
				if (camera.primary)
				{
					mainCamera = &camera.camera;
					cameraTransform = transform.getTransform();
					break;
				}
			}
		}

		if (mainCamera)
		{
			Renderer2D::beginScene(*mainCamera, cameraTransform);
			auto group = registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
			for (auto entity : group)
			{
				auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
				Renderer2D::drawQuad(transform.getTransform(), sprite.color);
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

	// Get primary camera entity
	Entity Scene::getPrimaryCameraEntity()
	{
		auto view = registry.view<CameraComponent>();
		for (auto entity : view)
		{
			const auto& camera = view.get<CameraComponent>(entity);
			if (camera.primary)
				return Entity{ entity, this };
		}

		return {};
	}

	// On update function for the editor
	void Scene::onUpdateEditor(Timestep timeStep, EditorCamera& camera)
	{
		Renderer2D::beginScene(camera);
		auto group = registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
		for (auto entity : group)
		{
			auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
			Renderer2D::drawQuad(transform.getTransform(), sprite.color);
		}
		Renderer2D::endScene();
	}

	// On component added (only called if component you are trying to add does not exist)
	template <typename T>
	void Scene::onComponentAdded(Entity entity, T& component)
	{
		static_assert(false);
	}

	// Templates for components
	template <>
	void Scene::onComponentAdded<CameraComponent>(Entity entity, CameraComponent& component)
	{
		component.camera.setViewportSize(viewportWidth, viewportHeight);
	}

	template <>
	void Scene::onComponentAdded<NativeScriptComponent>(Entity entity, NativeScriptComponent& component) {}

	template <>
	void Scene::onComponentAdded<SpriteRendererComponent>(Entity entity, SpriteRendererComponent& component) {}

	template <>
	void Scene::onComponentAdded<TagComponent>(Entity entity, TagComponent& component) {}

	template <>
	void Scene::onComponentAdded<TransformComponent>(Entity entity, TransformComponent& component) {}

}