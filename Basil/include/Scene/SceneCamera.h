/*
 * This header file declares a class for a Scene Camera.
 */
#pragma once

#include "Renderer/Camera.h"

namespace Basil
{
	class SceneCamera : public Camera
	{
		public:
			enum class CameraType { Perspective = 0, Orthographic = 1 };
		public:
			SceneCamera();
			~SceneCamera() = default;
			
			void setPerspective(float verticalFOV, float nearClip, float farClip);
			void setOrthographic(float size, float nearClip, float farClip);
			
			void setViewportSize(uint32_t width, uint32_t height);
			
			float getPerspectiveVerticalFOV() const;
			void setPerspectiveVerticalFOV(float verticalFOV);
			float getPerspectiveNearClip() const;
			void setPerspectiveNearClip(float nearClip);
			float getPerspectiveFarClip() const;
			void setPerspectiveFarClip(float farClip);

			float getOrthographicSize() const;
			void setOrthographicSize(float size);
			float getOrthographicNearClip() const;
			void setOrthographicNearClip(float nearClip);
			float getOrthographicFarClip() const;
			void setOrthographicFarClip(float farClip);
			
			CameraType getCameraType() const;
			void setCameraType(CameraType type);
		private:
			void recalculateProjection();

			CameraType cameraType;
			
			float perspectiveFOV;
			float perspectiveNear;
			float perspectiveFar;
			
			float orthographicSize;
			float orthographicNear;
			float orthographicFar;
			
			float aspectRatio;
	};
}