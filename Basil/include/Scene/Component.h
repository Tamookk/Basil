/*
 * This header file defines various components.
 */
#pragma once

#include "Scene/SceneCamera.h"
#include "Scene/ScriptableEntity.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

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

	struct SpriteRenderComponent
	{
		SpriteRenderComponent() = default;
		SpriteRenderComponent(const SpriteRenderComponent&) = default;
		SpriteRenderComponent(const glm::vec4& color) : color(color) {}
		glm::vec4 color{ 1.0f, 1.0f, 1.0f, 1.0f };
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
			glm::mat4 localRotation = glm::rotate(glm::mat4(1.0f), rotation.x, { 1, 0, 0 })
									* glm::rotate(glm::mat4(1.0f), rotation.y, { 0, 1, 0 })
									* glm::rotate(glm::mat4(1.0f), rotation.z, { 0, 0, 1 });

			return glm::translate(glm::mat4(1.0f), translation)
					* localRotation
					* glm::scale(glm::mat4(1.0f), scale);
		}

		glm::vec3 translation{ 0.0f, 0.0f, 0.0f };
		glm::vec3 rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 scale{ 1.0f, 1.0f, 1.0f };
	};
}