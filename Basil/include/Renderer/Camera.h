/*
 * Declares a generic camera class.
 */
#pragma once

#include <glm/glm.hpp>

namespace Basil
{
	class Camera
	{
		public:
			// TODO: move definition to own cpp file
			Camera() = default;
			Camera(const glm::mat4& projection) : projection(projection) {}
			virtual ~Camera() = default;
			const glm::mat4& getProjection() const { return projection; }
		protected:
			glm::mat4 projection = glm::mat4(1.0f);
	};
}