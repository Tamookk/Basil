#include "pch.h"
#include "Renderer/Renderer2D.h"
#include "Scene/Component.h"
#include "Scene/Entity.h"
#include "Scene/Scene.h"

#include <box2d/b2_body.h>
#include <box2d/b2_fixture.h>
#include <box2d/b2_polygon_shape.h>
#include <box2d/b2_world.h>
#include <glm/glm.hpp>

namespace Basil
{
	// Convert Basil Rigidbody type to Box2D type
	static b2BodyType BasilRigidbody2DTypeToBox2D(Rigidbody2DComponent::BodyType bodyType)
	{
		switch (bodyType)
		{
			case Rigidbody2DComponent::BodyType::Static:	return b2_staticBody;
			case Rigidbody2DComponent::BodyType::Dynamic:	return b2_dynamicBody;
			case Rigidbody2DComponent::BodyType::Kinematic:	return b2_kinematicBody;
		}
	}

	// Constructor
	Scene::Scene()
	{
		// Initialise variables
		viewportWidth = 0;
		viewportHeight = 0;
		box2DWorld = nullptr;
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

	// On runtime start
	void Scene::onRuntimeStart()
	{
		box2DWorld = new b2World({ 0.0f, -9.8f });
		auto view = registry.view<Rigidbody2DComponent>();
		for (auto e : view)
		{
			Entity entity = { e, this };
			auto& transform = entity.getComponent<TransformComponent>();
			auto& rb2d = entity.getComponent<Rigidbody2DComponent>();

			b2BodyDef bodyDef;
			bodyDef.type = BasilRigidbody2DTypeToBox2D(rb2d.type);
			bodyDef.position.Set(transform.translation.x, transform.translation.y);
			bodyDef.angle = transform.rotation.z;

			b2Body* body = box2DWorld->CreateBody(&bodyDef);
			body->SetFixedRotation(rb2d.fixedRotation);
			rb2d.runtimeBody = body;

			if (entity.hasComponent<BoxCollider2DComponent>())
			{
				auto& bc2d = entity.getComponent<BoxCollider2DComponent>();

				b2PolygonShape polygonShape;
				polygonShape.SetAsBox(bc2d.size.x * transform.scale.x, bc2d.size.y * transform.scale.y);

				b2FixtureDef fixtureDef;
				fixtureDef.shape = &polygonShape;
				fixtureDef.density = bc2d.density;
				fixtureDef.friction = bc2d.friction;
				fixtureDef.restitution = bc2d.restitution;
				fixtureDef.restitutionThreshold = bc2d.restitutionThreshold;
				body->CreateFixture(&fixtureDef);
			}
		}
	}

	// On runtime stop
	void Scene::onRuntimeStop()
	{
		delete box2DWorld;
		box2DWorld = nullptr;
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

		// Physics
		{
			// Step the physics world
			const int32_t velocityIterations = 6;
			const int32_t positionIterations = 6;
			box2DWorld->Step(timeStep, velocityIterations, positionIterations);

			// Retrieve transform from Box2D, update transform component with it
			auto view = registry.view<Rigidbody2DComponent>();
			for (auto e : view)
			{
				Entity entity = { e, this };
				auto& transform = entity.getComponent<TransformComponent>();
				auto& rb2d = entity.getComponent<Rigidbody2DComponent>();
				b2Body* body = (b2Body*)rb2d.runtimeBody;
				const auto& position = body->GetPosition();
				transform.translation.x = position.x;
				transform.translation.y = position.y;
				transform.rotation.z = body->GetAngle();
			}
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
				Renderer2D::drawSprite(transform.getTransform(), sprite, (int)entity);
			}
			Renderer2D::endScene();
		}
	}

	// On update function for the editor
	void Scene::onUpdateEditor(Timestep timeStep, EditorCamera& camera)
	{
		Renderer2D::beginScene(camera);
		auto group = registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
		for (auto entity : group)
		{
			auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
			Renderer2D::drawSprite(transform.getTransform(), sprite, (int)entity);
		}
		Renderer2D::endScene();
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
		if(viewportWidth > 0 && viewportHeight > 0)
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

	template <>
	void Scene::onComponentAdded<Rigidbody2DComponent>(Entity entity, Rigidbody2DComponent& component) {}

	template <>
	void Scene::onComponentAdded<BoxCollider2DComponent>(Entity entity, BoxCollider2DComponent& component) {}

}