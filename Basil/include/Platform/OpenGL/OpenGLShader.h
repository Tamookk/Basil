/*
 * OpenGL-specific implementation of Shader.h.
 */
#pragma once

#include <unordered_map>

#include "Renderer/Shader.h"

#include <glm/glm.hpp>

namespace Basil
{
	typedef unsigned int GLenum;	// Bad, remove eventually

	class OpenGLShader : public Shader
	{
		public:
			OpenGLShader(const std::string& filePath);
			OpenGLShader(const std::string& name, const std::string& vertexShaderSource, const std::string& fragmentShaderSource);
			~OpenGLShader();
			void bind() const override;
			void unbind() const override;
			const std::string& getName() const override;
			void setInt(const std::string& name, int value) override;
			void setIntArray(const std::string& name, int* values, uint32_t count) override;
			void setFloat(const std::string& name, float value) override;
			void setFloat3(const std::string& name, const glm::vec3& value) override;
			void setFloat4(const std::string& name, const glm::vec4& value) override;
			void setMat4(const std::string& name, const glm::mat4& matrix) override;
			void uploadUniformInt(const std::string& name, int value);
			void uploadUniformIntArray(const std::string& name, int* values, uint32_t count);
			void uploadUniformFloat(const std::string& name, float value);
			void uploadUniformFloat2(const std::string& name, const glm::vec2& value);
			void uploadUniformFloat3(const std::string& name, const glm::vec3& value);
			void uploadUniformFloat4(const std::string& name, const glm::vec4& value);
			void uploadUniformMat3(const std::string& name, const glm::mat3& value);
			void uploadUniformMat4(const std::string& name, const glm::mat4& matrix);
		private:
			std::string readFile(const std::string& filePath);
			std::unordered_map<GLenum, std::string> preProcess(const std::string& source);
			void compileOrGetVulkanBinaries(std::unordered_map<GLenum, std::string>& shaderSources);
			void compileOrGetOpenGLBinaries();
			void createProgram();
			void reflect(GLenum stage, const std::vector<uint32_t>& shaderData);

			uint32_t rendererID;
			std::string filePath;
			std::string name;

			std::unordered_map<GLenum, std::vector<uint32_t>> VulkanSPIRV;
			std::unordered_map<GLenum, std::vector<uint32_t>> OpenGLSPIRV;

			std::unordered_map<GLenum, std::string> OpenGLSourceCode;
	};
}