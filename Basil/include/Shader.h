/*
 * This header file declares a class for creating an OpenGL shader.
 */
#pragma once

#include <string>

#include <glm/glm.hpp>

namespace Basil
{
	class Shader
	{
		public:
			Shader(const std::string& vertexShaderSource, const std::string& fragmentShaderSource);
			~Shader();
			void bind() const;
			void unbind() const;
			void uploadUniformMat4(const std::string& name, const glm::mat4& matrix);
		private:
			uint32_t rendererID;
	};
}
