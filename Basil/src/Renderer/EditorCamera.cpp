#include "pch.h"
#include "Core/Input.h"
#include "Core/KeyCodes.h"
#include "Core/MouseCodes.h"
#include "Renderer/EditorCamera.h"

#include <GLFW/glfw3.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace Basil
{
	// Constructor
	EditorCamera::EditorCamera(float fov, float aspectRatio, float nearClip, float farClip)
		: Camera(glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip))
	{
		// Set variables
		FOV = fov;
		this->aspectRatio = aspectRatio;
		this->nearClip = nearClip;
		this->farClip = farClip;

		distance = 10.0f;
		pitch = 0.0f;
		yaw = 0.0f;

		viewportWidth = 1280.0f;
		viewportHeight = 720.0f;

		// Update view matrix
		updateView();
	}

	// On update function
	void EditorCamera::onUpdate(Timestep timeStep)
	{
		// If left alt being pressed
		if (Input::isKeyPressed(Key::LeftAlt))
		{
			// Calculate mouse delta
			const glm::vec2& mouse{ Input::getMouseX(), Input::getMouseY() };
			glm::vec2 delta = (mouse - initialMousePosition) * 0.003f;
			initialMousePosition = mouse;

			// Pan, rotate, zoom
			if (Input::isMouseButtonPressed(Mouse::ButtonMiddle))
				mousePan(delta);
			else if (Input::isMouseButtonPressed(Mouse::ButtonLeft))
				mouseRotate(delta);
			else if (Input::isMouseButtonPressed(Mouse::ButtonRight))
				mouseZoom(delta.y);
		}

		// Update the view matrix
		updateView();
	}

	// On event function
	void EditorCamera::onEvent(Event& e)
	{
		// Dispatch mouse scroll events
		EventDispatcher dispatcher(e);
		dispatcher.dispatch<MouseScrolledEvent>(BIND_EVENT(EditorCamera::onMouseScroll));
	}

	// Return distance
	float EditorCamera::getDistance() const
	{
		return distance;
	}

	// Set distance
	void EditorCamera::setDistance(float distance)
	{
		this->distance = distance;
	}
	void EditorCamera::setViewportSize(float width, float height)
	{
		viewportWidth = width;
		viewportHeight = height;
		updateProjection();
	}

	// Return view matrix
	const glm::mat4& EditorCamera::getViewMatrix() const
	{
		return viewMatrix;
	}

	// Return view projection matrix
	glm::mat4 EditorCamera::getViewProjection() const
	{
		return projection * viewMatrix;
	}

	// Return up direction
	glm::vec3 EditorCamera::getUpDirection() const
	{
		return glm::rotate(getOrientation(), glm::vec3(0.0f, 1.0f, 0.0f));
	}

	// Get right direction
	glm::vec3 EditorCamera::getRightDirection() const
	{
		return glm::rotate(getOrientation(), glm::vec3(1.0f, 0.0f, 0.0f));
	}

	// Get forward direction
	glm::vec3 EditorCamera::getForwardDirection() const
	{
		return glm::rotate(getOrientation(), glm::vec3(0.0f, 0.0f, -1.0f));
	}

	// Return position
	const glm::vec3& EditorCamera::getPosition() const
	{
		return position;
	}

	// Return camera orientation
	glm::quat EditorCamera::getOrientation() const
	{
		return glm::quat(glm::vec3(-pitch, -yaw, 0.0f));
	}

	// Return pitch
	float EditorCamera::getPitch() const
	{
		return pitch;
	}

	// Return yaw
	float EditorCamera::getYaw() const
	{
		return yaw;
	}

	// Update the projection matrix
	void EditorCamera::updateProjection()
	{
		aspectRatio = viewportWidth / viewportHeight;
		projection = glm::perspective(glm::radians(FOV), aspectRatio, nearClip, farClip);
	}

	// Update the view matrix
	void EditorCamera::updateView()
	{
		// yaw = pitch = 0.0f; // Lock camera rotation
		position = calculatePosition();

		glm::quat orientation = getOrientation();
		viewMatrix = glm::translate(glm::mat4(1.0f), position) * glm::toMat4(orientation);
		viewMatrix = glm::inverse(viewMatrix);
	}

	// On mouse scroll function
	bool EditorCamera::onMouseScroll(MouseScrolledEvent & e)
	{
		// Calculate delta and zoom camera
		float delta = e.getYOffset() * 0.1f;
		mouseZoom(delta);
		
		// Update the view matrix
		updateView();

		// Return false (to prevent event blocking)
		return false;
	}

	// Mouse pan function
	void EditorCamera::mousePan(const glm::vec2 & delta)
	{
		auto [xSpeed, ySpeed] = panSpeed();
		focalPoint += -getRightDirection() * delta.x * xSpeed * distance;
		focalPoint += getUpDirection() * delta.y * ySpeed * distance;
	}

	// Mouse rotate function
	void EditorCamera::mouseRotate(const glm::vec2 & delta)
	{
		float yawSign = getUpDirection().y < 0 ? -1.0f : 1.0f;
		yaw += yawSign * delta.x * rotationSpeed();
		pitch += delta.y * rotationSpeed();
	}

	// Mouse zoom function
	void EditorCamera::mouseZoom(float delta)
	{
		distance -= delta * zoomSpeed();
		if (distance < 1.0f)
		{
			focalPoint += getForwardDirection();
			distance = 1.0f;
		}
	}

	// Calculate camera position
	glm::vec3 EditorCamera::calculatePosition() const
	{
		return focalPoint - getForwardDirection() * distance;
	}

	// Set the pan speed
	std::pair<float, float> EditorCamera::panSpeed() const
	{
		// Magic numbers everywhere
		float x = std::min(viewportWidth / 1000.0f, 2.4f);
		float xFactor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;

		float y = std::min(viewportHeight / 1000.0f, 2.4f);
		float yFactor = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;

		return { xFactor, yFactor };
	}

	// Return the rotation speed
	float EditorCamera::rotationSpeed() const
	{
		// Magic number
		return 0.8f;
	}

	// Get the zoom speed
	float EditorCamera::zoomSpeed() const
	{
		float distance = this->distance * 0.2f;
		distance = std::max(distance, 0.0f);
		float speed = distance * distance;
		speed = std::min(speed, 100.0f);
		return speed;
	}
}