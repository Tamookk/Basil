#include "pch.h"
#include "Renderer/Renderer2D.h"
#include "Scene/Component.h"
#include "Scene/Entity.h"
#include "Scene/Scene.h"
#include "Scene/ScriptableEntity.h"
#include "Scripting/ScriptEngine.h"

#include <box2d/b2_body.h>
#include <box2d/b2_fixture.h>
#include <box2d/b2_circle_shape.h>
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
		delete box2DWorld;
	}

	// Copy component
	template<typename... Component>
	static void copyComponent(entt::registry& dst, entt::registry & src, const std::unordered_map<UUID, entt::entity>& enttMap)
	{
		([&]()
			{
				// Get component view of source entity
				auto view = src.view<Component>();

				// For each source entity
				for (auto srcEntity : view)
				{
					// Copy the destination entity
					entt::entity dstEntity = enttMap.at(src.get<IDComponent>(srcEntity).id);

					// Get reference to source component
					auto& srcComponent = src.get<Component>(srcEntity);

					// Replace component in destination with one from source
					dst.emplace_or_replace<Component>(dstEntity, srcComponent);
				}
			}(), ...);
	}

	// Copy component group
	template<typename... Component>
	static void copyComponent(ComponentGroup<Component...>, entt::registry& dst, entt::registry& src, const std::unordered_map<UUID, entt::entity>& enttMap)
	{
		// Call copy component for each component in the group
		copyComponent<Component...>(dst, src, enttMap);
	}

	// Copy component if exists
	template<typename... Component>
	static void copyComponentIfExists(Entity dst, Entity src)
	{
		([&]()
			{
				if (src.hasComponent<Component>())
					dst.addOrReplaceComponent<Component>(src.getComponent<Component>());

			}(), ...);
	}

	// Copy component if exists for component group
	template<typename... Component>
	static void copyComponentIfExists(ComponentGroup<Component...>, Entity dst, Entity src)
	{
		copyComponentIfExists<Component...>(dst, src);
	}

	// Copy a scene
	Shared<Scene> Scene::copy(Shared<Scene> other)
	{
		// Create the new scene
		Shared<Scene> newScene = makeShared<Scene>();

		// Copy the viewport size
		newScene->viewportWidth = other->viewportWidth;
		newScene->viewportHeight = other->viewportHeight;

		// Create entities in new scene
		auto& srcSceneRegistry = other->registry;
		auto& dstSceneRegistry = newScene->registry;
		std::unordered_map<UUID, entt::entity> enttMap;

		auto idView = srcSceneRegistry.view<IDComponent>();
		for (auto e : idView)
		{
			UUID uuid = srcSceneRegistry.get<IDComponent>(e).id;
			const auto& name = srcSceneRegistry.get<TagComponent>(e).tag;
			Entity newEntity = newScene->createEntityWithUUID(uuid, name);
			enttMap[uuid] = (entt::entity)newEntity;
		}

		// Copy components (except IDComponent and TagComponent)
		copyComponent(AllComponents{}, dstSceneRegistry, srcSceneRegistry, enttMap);

		return newScene;
	}

	// Create an entity
	Entity Scene::createEntity(const std::string& name)
	{
		return createEntityWithUUID(UUID(), name);
	}

	// Create an entity
	Entity Scene::createEntityWithUUID(UUID uuid, const std::string& name)
	{
		// Create an entity
		Entity entity = { registry.create(), this };

		// Add an ID, transform and tag component
		entity.addComponent<IDComponent>(uuid);
		entity.addComponent<TransformComponent>();
		auto& tag = entity.addComponent<TagComponent>();
		tag.tag = name.empty() ? "Entity" : name;

		// Add entity to the entity map
		entityMap[uuid] = entity;

		// Return the entity
		return entity;
	}

	// Return entity by UUID
	Entity Scene::getEntityByUUID(UUID uuid)
	{
		// If entity is in entity map, return a new instance of it
		if (entityMap.find(uuid) != entityMap.end())
			return { entityMap.at(uuid), this };

		// Otherwise return an empty entity
		return {};
	}

	// Destroy an entity
	void Scene::destroyEntity(Entity entity)
	{
		registry.destroy(entity);
		entityMap.erase(entity.getUUID());
	}

	// On runtime start
	void Scene::onRuntimeStart()
	{
		onPhysics2DStart();

		// Scripting
		{
			ScriptEngine::onRuntimeStart(this);

			// Instantiate all script entities
			auto view = registry.view<ScriptComponent>();
			for (auto e : view)
			{
				Entity entity = { e, this };
				ScriptEngine::onCreateEntity(entity);
			}
		}
	}

	// On runtime stop
	void Scene::onRuntimeStop()
	{
		onPhysics2DStop();

		ScriptEngine::onRuntimeStop();
	}

	// On simulation start
	void Scene::onSimulationStart()
	{
		onPhysics2DStart();
	}

	// On simulation stop
	void Scene::onSimulationStop()
	{
		onPhysics2DStop();
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
			// C# scripts
			{
				auto view = registry.view<ScriptComponent>();
				for (auto e : view)
				{
					Entity entity = { e, this };
					ScriptEngine::onUpdateEntity(entity, timeStep);
				}
			}

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
			
			// Draw sprites
			{
				auto group = registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
				for (auto entity : group)
				{
					auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
					Renderer2D::drawSprite(transform.getTransform(), sprite, (int)entity);
				}
			}

			// Draw circles
			{
				auto view = registry.view<TransformComponent, CircleRendererComponent>();
				for (auto entity : view)
				{
					auto [transform, circle] = view.get<TransformComponent, CircleRendererComponent>(entity);
					Renderer2D::drawCircle(transform.getTransform(), circle.color, circle.thickness, circle.fade, (int)entity);
				}
			}

			Renderer2D::endScene();
		}
	}

	// On update function for the editor
	void Scene::onUpdateEditor(Timestep timeStep, EditorCamera& camera)
	{
		renderScene(camera);
	}

	// On simulation update
	void Scene::onUpdateSimulation(Timestep timeStep, EditorCamera& camera)
	{
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

		renderScene(camera);
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

	// Duplicate an entity
	void Scene::duplicateEntity(Entity entity)
	{
		Entity newEntity = createEntity(entity.getName());
		copyComponentIfExists(AllComponents{}, newEntity, entity);
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

	// On 2D physics start
	void Scene::onPhysics2DStart()
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

			if (entity.hasComponent<CircleCollider2DComponent>())
			{
				// Get the circle collider 2D component
				auto& cc2d = entity.getComponent<CircleCollider2DComponent>();

				// Create the b2CircleShape object and set its parameters
				b2CircleShape circleShape;
				circleShape.m_p.Set(cc2d.offset.x, cc2d.offset.y);
				circleShape.m_radius = transform.scale.x * cc2d.radius;

				// Set b2FixtureDef object parameters
				b2FixtureDef fixtureDef;
				fixtureDef.shape = &circleShape;
				fixtureDef.density = cc2d.density;
				fixtureDef.friction = cc2d.friction;
				fixtureDef.restitution = cc2d.restitution;
				fixtureDef.restitutionThreshold = cc2d.restitutionThreshold;
				body->CreateFixture(&fixtureDef);
			}
		}
	}

	// On 2D physics stop
	void Scene::onPhysics2DStop()
	{
		delete box2DWorld;
		box2DWorld = nullptr;
	}

	// Render scene as seen from camera
	void Scene::renderScene(EditorCamera& camera)
	{
		Renderer2D::beginScene(camera);

		// Draw sprites
		{
			auto group = registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
			for (auto entity : group)
			{
				auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
				Renderer2D::drawSprite(transform.getTransform(), sprite, (int)entity);
			}
		}

		// Draw circles
		{
			auto view = registry.view<TransformComponent, CircleRendererComponent>();
			for (auto entity : view)
			{
				auto [transform, circle] = view.get<TransformComponent, CircleRendererComponent>(entity);
				Renderer2D::drawCircle(transform.getTransform(), circle.color, circle.thickness, circle.fade, (int)entity);
			}
		}

		Renderer2D::endScene();
	}

	// On component added (only called if component you are trying to add does not exist)
	template <typename T>
	void Scene::onComponentAdded(Entity entity, T& component)
	{
		static_assert(sizeof(T) == 0);
	}

	// Templates for components
	template <>
	void Scene::onComponentAdded<IDComponent>(Entity entity, IDComponent& component) {}

	template <>
	void Scene::onComponentAdded<CameraComponent>(Entity entity, CameraComponent& component)
	{
		if(viewportWidth > 0 && viewportHeight > 0)
			component.camera.setViewportSize(viewportWidth, viewportHeight);
	}

	template <>
	void Scene::onComponentAdded<ScriptComponent>(Entity entity, ScriptComponent& component) {}

	template <>
	void Scene::onComponentAdded<NativeScriptComponent>(Entity entity, NativeScriptComponent& component) {}

	template <>
	void Scene::onComponentAdded<TagComponent>(Entity entity, TagComponent& component) {}

	template <>
	void Scene::onComponentAdded<TransformComponent>(Entity entity, TransformComponent& component) {}

	template <>
	void Scene::onComponentAdded<SpriteRendererComponent>(Entity entity, SpriteRendererComponent& component) {}

	template <>
	void Scene::onComponentAdded<CircleRendererComponent>(Entity entity, CircleRendererComponent& component) {}

	template <>
	void Scene::onComponentAdded<Rigidbody2DComponent>(Entity entity, Rigidbody2DComponent& component) {}

	template <>
	void Scene::onComponentAdded<BoxCollider2DComponent>(Entity entity, BoxCollider2DComponent& component) {}

	template <>
	void Scene::onComponentAdded<CircleCollider2DComponent>(Entity entity, CircleCollider2DComponent& component) {}

}