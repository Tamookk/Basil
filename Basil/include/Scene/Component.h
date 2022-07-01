/*
 * This header file defines various components.
 */
#pragma once

#include "Core/UUID.h"
#include "Renderer/Texture2D.h"
#include "Scene/SceneCamera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace Basil
{
	struct CameraComponent
	{
		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
		SceneCamera camera;
		bool primary = true;
		bool fixedAspectRatio = false;
	};

	struct IDComponent
	{
		UUID id;

		IDComponent() = default;
		IDComponent(const IDComponent&) = default;
	};

	class ScriptableEntity;	// Forward declaration
	struct NativeScriptComponent
	{
		ScriptableEntity* instance = nullptr;

		ScriptableEntity*(*instantiateScript)();
		void (*destroyScript)(NativeScriptComponent*);

		template <typename T>
		void bind()
		{
			instantiateScript = []() { return static_cast<ScriptableEntity*>(new T()); };
			destroyScript = [](NativeScriptComponent* nsc) { delete nsc->instance; nsc->instance = nullptr; };
		}
	};

	struct TagComponent
	{
		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag) : tag(tag) {}
		std::string tag;
	};

	struct TransformComponent
	{
		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::vec3& translation) : translation(translation) {}
		
		glm::mat4 getTransform() const
		{
			glm::mat4 localRotation = glm::toMat4(glm::quat(rotation));

			return glm::translate(glm::mat4(1.0f), translation)
					* localRotation
					* glm::scale(glm::mat4(1.0f), scale);
		}

		glm::vec3 translation{ 0.0f, 0.0f, 0.0f };
		glm::vec3 rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 scale{ 1.0f, 1.0f, 1.0f };
	};


	// == 2D Rendering ==
	struct SpriteRendererComponent
	{
		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const glm::vec4& color) : color(color) {}
		float tilingFactor = 1.0f;
		glm::vec4 color{ 1.0f, 1.0f, 1.0f, 1.0f };
		Shared<Texture2D> texture;
	};

	struct CircleRendererComponent
	{
		glm::vec4 color{ 1.0f, 1.0f, 1.0f, 1.0f };
		float thickness = 1.0f;
		float fade = 0.005f;


		CircleRendererComponent() = default;
		CircleRendererComponent(const CircleRendererComponent&) = default;
	};


	// == Physics ==
	struct Rigidbody2DComponent
	{
		enum class BodyType { Static = 0, Dynamic, Kinematic };

		Rigidbody2DComponent() = default;
		Rigidbody2DComponent(const Rigidbody2DComponent& other) = default;

		BodyType type = BodyType::Static;
		bool fixedRotation = false;
		void* runtimeBody = nullptr;
	};

	struct BoxCollider2DComponent
	{
		BoxCollider2DComponent() = default;
		BoxCollider2DComponent(const BoxCollider2DComponent& other) = default;

		glm::vec2 offset = { 0.0f, 0.0f };
		glm::vec2 size = { 0.5f, 0.5f };
		float density = 1.0f;
		float friction = 0.5f;
		float restitution = 0.0f;
		float restitutionThreshold = 0.5f;
		void* runtimeBody = nullptr;
	};

	struct CircleCollider2DComponent
	{
		CircleCollider2DComponent() = default;
		CircleCollider2DComponent(const CircleCollider2DComponent& other) = default;

		glm::vec2 offset = { 0.0f, 0.0f };
		float radius = 0.5f;
		float density = 1.0f;
		float friction = 0.5f;
		float restitution = 0.0f;
		float restitutionThreshold = 0.5f;
		void* runtimeBody = nullptr;
	};

	template<typename... Component>
	struct ComponentGroup
	{

	};

	using AllComponents =
		ComponentGroup<TransformComponent, SpriteRendererComponent, CameraComponent, NativeScriptComponent,
						Rigidbody2DComponent, BoxCollider2DComponent, CircleCollider2DComponent>;
}