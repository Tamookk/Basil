/*
 * This header file declares the camera used by the editor.
 */
#pragma once

#include "Core/Timestep.h"
#include "Events/Event.h"
#include "Events/MouseEvent.h"
#include "Renderer/Camera.h"

#include <glm/glm.hpp>

namespace Basil
{
	class EditorCamera : public Camera
	{
	public:
		EditorCamera() = default;
		EditorCamera(float fov, float aspectRatio, float nearClip, float farClip);

		void onUpdate(Timestep timeStep);
		void onEvent(Event& e);

		float getDistance() const;
		void setDistance(float distance);

		void setViewportSize(float width, float height);

		const glm::mat4& getViewMatrix() const;
		glm::mat4 getViewProjection() const;

		glm::vec3 getUpDirection() const;
		glm::vec3 getRightDirection() const;
		glm::vec3 getForwardDirection() const;
		const glm::vec3& getPosition() const;
		glm::quat getOrientation() const;

		float getPitch() const;
		float getYaw() const;

	private:
		void updateProjection();
		void updateView();

		bool onMouseScroll(MouseScrolledEvent& e);

		void mousePan(const glm::vec2& delta);
		void mouseRotate(const glm::vec2& delta);
		void mouseZoom(float delta);

		glm::vec3 calculatePosition() const;

		std::pair<float, float> panSpeed() const;
		float rotationSpeed() const;
		float zoomSpeed() const;

		float FOV;
		float aspectRatio;
		float nearClip;
		float farClip;

		glm::mat4 viewMatrix;
		glm::vec3 position = { 0.0f, 0.0f, 0.0f };
		glm::vec3 focalPoint = { 0.0f, 0.0f, 0.0f };

		glm::vec2 initialMousePosition = { 0.0f, 0.0f };

		float distance;
		float pitch;
		float yaw;

		float viewportWidth;
		float viewportHeight;
	};
}