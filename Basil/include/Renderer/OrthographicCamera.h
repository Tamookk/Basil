/*
 * This header file declares an orthographic camera class.
 */
#pragma once

#include <glm/glm.hpp>

namespace Basil
{
	class OrthographicCamera
	{
		public:
			OrthographicCamera(float left, float right, float bottom, float top);
			const glm::vec3& getPosition() const;
			void setPosition(const glm::vec3& position);
			float getRotation() const;
			void setRotation(float rotation);
			const glm::mat4& getProjectionMatrix() const;
			const glm::mat4& getViewMatrix() const;
			const glm::mat4& getViewProjectionMatrix() const;
		private:
			void recalculateViewMatrix();
			glm::mat4 projMatrix;
			glm::mat4 viewMatrix;
			glm::mat4 viewProjectionMatrix;
			glm::vec3 position;
			float rotation;
	};
}