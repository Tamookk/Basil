/*
 * This header file defines various components.
 */
#pragma once

#include "Renderer/Camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Basil
{
	struct CameraComponent
	{
		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
		CameraComponent(const glm::mat4& projection) : camera(projection) {}
		Camera camera;
		bool primary = true;
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
		TransformComponent(const glm::mat4& transform) : transform(transform) {}
		const void updateTransform() { transform = glm::translate(glm::mat4(1.0f), position) * glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f)) * glm::scale(glm::mat4(1.0f), scale); }
		operator glm::mat4& () { return transform; }
		operator const glm::mat4& () const { return transform; }
		glm::mat4 transform{ 1.0f };
		glm::vec3 position{ 0.0f, 0.0f, 0.0f };
		float rotation = 0.0f;
		glm::vec3 scale{ 1.0f, 1.0f, 1.0f };
	};
}