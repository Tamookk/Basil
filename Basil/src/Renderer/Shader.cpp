#include "pch.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "Renderer/Renderer.h"
#include "Renderer/Shader.h"


namespace Basil
{
	// Create shader depending on platform and graphics API
	Shared<Shader> Shader::create(const std::string& filePath)
	{
		switch (Renderer::getAPI())
		{
			case RendererAPI::API::None:
				ASSERT(false, "No renderer API!");
				return nullptr;
			case RendererAPI::API::OpenGL:
				return std::make_shared<OpenGLShader>(filePath);
		}

		ASSERT(false, "Unknown renderer API.");
		return nullptr;
	}

	Shared<Shader> Shader::create(const std::string& name, const std::string& vertexSource, const std::string& fragmentSource)
	{
		switch (Renderer::getAPI())
		{
			case RendererAPI::API::None:
				ASSERT(false, "No renderer API!");
				return nullptr;
			case RendererAPI::API::OpenGL:
				return std::make_shared<OpenGLShader>(name, vertexSource, fragmentSource);
		}

		ASSERT(false, "Unknown renderer API.");
		return nullptr;
	}
}