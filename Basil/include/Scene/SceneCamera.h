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
			SceneCamera();
			~SceneCamera() = default;
			void setOrthographic(float size, float nearClip, float farClip);
			void setViewportSize(uint32_t width, uint32_t height);
			float getOrthographicSize() const;
			void setOrthographicSize(float size);
		private:
			void recalculateProjection();
			float orthographicSize;
			float orthographicNear;
			float orthographicFar;
			float aspectRatio;
	};
}