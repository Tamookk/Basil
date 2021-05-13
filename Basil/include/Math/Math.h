/*
 * This header file declares a wrapper for glm, and some functions to suit the project's needs.
 */
#pragma once

#include <glm/glm.hpp>

namespace Basil::Math
{
	bool decomposeTransform(const glm::mat4& transform, glm::vec3& outTranslation, glm::vec3& outRotation, glm::vec3& outScale);
}