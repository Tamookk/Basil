#include "pch.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "Renderer/Renderer.h"
#include "Renderer/Shader.h"


namespace Basil
{
	// Create shader depending on platform and graphics API
	Shader* Shader::create(const std::string& filePath)
	{
		switch (Renderer::getAPI())
		{
			case RendererAPI::API::None:
				ASSERT(false, "No renderer API!");
				return nullptr;
			case RendererAPI::API::OpenGL:
				return new OpenGLShader(filePath);
		}

		ASSERT(false, "Unknown renderer API.");
		return nullptr;
	}

	Shader* Shader::create(const std::string& vertexSource, const std::string& fragmentSource)
	{
		switch (Renderer::getAPI())
		{
			case RendererAPI::API::None:
				ASSERT(false, "No renderer API!");
				return nullptr;
			case RendererAPI::API::OpenGL:
				return new OpenGLShader(vertexSource, fragmentSource);
		}

		ASSERT(false, "Unknown renderer API.");
		return nullptr;
	}
}