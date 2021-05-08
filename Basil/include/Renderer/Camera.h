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
			Camera(const glm::mat4& projection) : projection(projection) {}
			const glm::mat4& getProjection() const { return projection; }
		private:
			glm::mat4 projection;
	};
}