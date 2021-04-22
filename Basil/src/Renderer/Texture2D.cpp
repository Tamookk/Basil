#include "pch.h"
#include "Platform/OpenGL/OpenGLTexture2D.h"
#include "Renderer/Renderer.h"
#include "Renderer/Texture2D.h"

namespace Basil
{
	Shared<Texture2D> Texture2D::create(const std::string& path)
	{
		switch (Renderer::getAPI())
		{
			case RendererAPI::API::None:
				ASSERT(false, "No renderer API!");
				return nullptr;
			case RendererAPI::API::OpenGL:
				return makeShared<OpenGLTexture2D>(path);
		}

		ASSERT(false, "Unknown renderer API.");
		return nullptr;
	}

	Shared<Texture2D> Texture2D::create(uint32_t width, uint32_t height)
	{
		switch (Renderer::getAPI())
		{
			case RendererAPI::API::None:
				ASSERT(false, "No renderer API!");
				return nullptr;
			case RendererAPI::API::OpenGL:
				return makeShared<OpenGLTexture2D>(width, height);
		}

		ASSERT(false, "Unknown renderer API.");
		return nullptr;
	}
}