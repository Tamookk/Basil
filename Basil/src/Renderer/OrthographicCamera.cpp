#include "pch.h"
#include "Renderer/OrthographicCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Basil
{
	// Constructor
	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
	{
		projMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
		viewMatrix = glm::mat4(1.0f);
		viewProjectionMatrix = projMatrix * viewMatrix;
		position = { 0.0f, 0.0f, 0.0f };
		rotation = 0.0f;
	}

	// Get position vector
	const glm::vec3& OrthographicCamera::getPosition() const
	{
		return position;
	}

	// Set position vector
	void OrthographicCamera::setPosition(const glm::vec3& position)
	{
		this->position = position;
		recalculateViewMatrix();
	}

	// Get rotation
	float OrthographicCamera::getRotation() const
	{
		return rotation;
	}

	// Set rotation
	void OrthographicCamera::setRotation(float rotation)
	{
		this->rotation = rotation;
		recalculateViewMatrix();
	}

	// Set projection matrix
	void OrthographicCamera::setProjectionMatrix(float left, float right, float bottom, float top)
	{
		projMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
		viewProjectionMatrix = projMatrix * viewMatrix;
	}

	// Return projection matrix
	const glm::mat4& OrthographicCamera::getProjectionMatrix() const
	{
		return projMatrix;
	}

	// Return view matrix
	const glm::mat4& OrthographicCamera::getViewMatrix() const
	{
		return viewMatrix;
	}

	// Return view projection matrix
	const glm::mat4& OrthographicCamera::getViewProjectionMatrix() const
	{
		return viewProjectionMatrix;
	}

	// Recalculate the view matrix
	void OrthographicCamera::recalculateViewMatrix()
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
		viewMatrix = glm::inverse(transform);
		viewProjectionMatrix = projMatrix * viewMatrix;
	}
}