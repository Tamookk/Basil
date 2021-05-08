#include "pch.h"
#include "Scene/SceneCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Basil
{
	// Constructor
	SceneCamera::SceneCamera()
	{
		// Initialise variables
		orthographicSize = 10.0f;
		orthographicNear = -1.0f;
		orthographicFar = 1.0f;
		aspectRatio = 0.0f;

		// Recalculate projection matrix
		recalculateProjection();
	}

	// Set orthographic parameters
	void SceneCamera::setOrthographic(float size, float nearClip, float farClip)
	{
		orthographicSize = size;
		orthographicNear = nearClip;
		orthographicFar = farClip;
		recalculateProjection();
	}

	// Set viewport size
	void SceneCamera::setViewportSize(uint32_t width, uint32_t height)
	{
		aspectRatio = (float)width / (float)height;
		recalculateProjection();
	}

	// Set orthographic size
	float SceneCamera::getOrthographicSize() const
	{
		return orthographicSize;
	}

	// Get orthographic size
	void SceneCamera::setOrthographicSize(float size)
	{
		orthographicSize = size;
		recalculateProjection();
	}
	
	// Recalculate the view projection matrix
	void SceneCamera::recalculateProjection()
	{
		float orthoLeft = -orthographicSize * aspectRatio * 0.5f;
		float orthoRight = orthographicSize * aspectRatio * 0.5f;
		float orthoBottom = -orthographicSize * 0.5f;
		float orthoTop = orthographicSize * 0.5f;

		projection = glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoTop, orthographicNear, orthographicFar);
	}
}