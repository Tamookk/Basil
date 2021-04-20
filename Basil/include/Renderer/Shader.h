/*
 * This header file declares a class for creating an OpenGL shader.
 */
#pragma once

#include <string>

#include "Core/Core.h"

#include <glm/glm.hpp>

namespace Basil
{
	class Shader
	{
		public:
			virtual ~Shader() {}
			virtual void bind() const = 0;
			virtual void unbind() const = 0;
			virtual const std::string& getName() const = 0;
			virtual void setInt(const std::string& name, int value) = 0;
			virtual void setFloat3(const std::string& name, const glm::vec3& value) = 0;
			virtual void setFloat4(const std::string& name, const glm::vec4& value) = 0;
			virtual void setMat4(const std::string& name, const glm::mat4& matrix) = 0;
			static Shared<Shader> create(const std::string& filePath);
			static Shared<Shader> create(const std::string& name, const std::string& vertexSource, const std::string& fragmentSource);
	};
}
