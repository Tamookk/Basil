#include "pch.h"
#include "Scene/SceneCamera.h"
#include "Scene/ScriptableEntity.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Basil
{
	// Constructor
	SceneCamera::SceneCamera()
	{
		// Initialise variables
		cameraType = CameraType::Orthographic;

		perspectiveFOV = glm::radians(45.0f);
		perspectiveNear = 0.01f;
		perspectiveFar = 1000.0f;

		orthographicSize = 10.0f;
		orthographicNear = -1.0f;
		orthographicFar = 1.0f;

		aspectRatio = 0.0f;

		// Recalculate projection matrix
		recalculateProjection();
	}

	// Set perspective parameters
	void SceneCamera::setPerspective(float verticalFOV, float nearClip, float farClip)
	{
		cameraType = CameraType::Perspective;
		perspectiveFOV = verticalFOV;
		perspectiveNear = nearClip;
		perspectiveFar = farClip;
		recalculateProjection();
	}

	// Set orthographic parameters
	void SceneCamera::setOrthographic(float size, float nearClip, float farClip)
	{
		cameraType = CameraType::Orthographic;
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

	// Get perspective FOV
	float SceneCamera::getPerspectiveVerticalFOV() const
	{
		return perspectiveFOV;
	}

	// Set perspective FOV
	void SceneCamera::setPerspectiveVerticalFOV(float verticalFOV)
	{
		perspectiveFOV = verticalFOV;
		recalculateProjection();
	}

	// Get perspective near clip
	float SceneCamera::getPerspectiveNearClip() const
	{
		return perspectiveNear;
	}

	// Set perspective near clip
	void SceneCamera::setPerspectiveNearClip(float nearClip)
	{
		perspectiveNear = nearClip;
		recalculateProjection();
	}
	
	// Get perspective far clip
	float SceneCamera::getPerspectiveFarClip() const
	{
		return perspectiveFar;
	}

	// Set perspective far clip
	void SceneCamera::setPerspectiveFarClip(float farClip)
	{
		perspectiveFar = farClip;
		recalculateProjection();
	}

	// Get orthographic size
	float SceneCamera::getOrthographicSize() const
	{
		return orthographicSize;
	}

	// Set orthographic size
	void SceneCamera::setOrthographicSize(float size)
	{
		orthographicSize = size;
		recalculateProjection();
	}

	// Get orthographic near clip
	float SceneCamera::getOrthographicNearClip() const
	{
		return orthographicNear;
	}

	// Set orthographic near clip
	void SceneCamera::setOrthographicNearClip(float nearClip)
	{
		orthographicNear = nearClip;
		recalculateProjection();
	}

	// Get orthographic far clip
	float SceneCamera::getOrthographicFarClip() const
	{
		return orthographicFar;
	}

	// Set orthographic far clip
	void SceneCamera::setOrthographicFarClip(float farClip)
	{
		orthographicFar = farClip;
		recalculateProjection();
	}

	// Return the camera type
	SceneCamera::CameraType SceneCamera::getCameraType() const
	{
		return cameraType;
	}

	// Set the camera type
	void SceneCamera::setCameraType(SceneCamera::CameraType type)
	{
		cameraType = type;
		recalculateProjection();
	}
	
	// Recalculate the view projection matrix
	void SceneCamera::recalculateProjection()
	{
		if (cameraType == CameraType::Perspective)
			projection = glm::perspective(perspectiveFOV, aspectRatio, perspectiveNear, perspectiveFar);
		else
		{
			float orthoLeft = -orthographicSize * aspectRatio * 0.5f;
			float orthoRight = orthographicSize * aspectRatio * 0.5f;
			float orthoBottom = -orthographicSize * 0.5f;
			float orthoTop = orthographicSize * 0.5f;

			projection = glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoTop, orthographicNear, orthographicFar);
		}
	}
}