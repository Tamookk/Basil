/*
 * OpenGL-specific implementation of Shader.h
 */
#pragma once

#include "Shader.h"

#include <glm/glm.hpp>

namespace Basil
{
	class OpenGLShader : public Shader
	{
		public:
			OpenGLShader(const std::string& vertexShaderSource, const std::string& fragmentShaderSource);
			~OpenGLShader();
			void bind() const override;
			void unbind() const override;
			void uploadUniformInt(const std::string& name, int value);
			void uploadUniformFloat(const std::string& name, float value);
			void uploadUniformFloat2(const std::string& name, const glm::vec2& value);
			void uploadUniformFloat3(const std::string& name, const glm::vec3& value);
			void uploadUniformFloat4(const std::string& name, const glm::vec4& value);
			void uploadUniformMat3(const std::string& name, const glm::mat3& value);
			void uploadUniformMat4(const std::string& name, const glm::mat4& matrix);
		private:
			uint32_t rendererID;
	};
}