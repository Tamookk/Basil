#include "pch.h"
#include "OpenGLTexture2D.h"
#include "Renderer.h"
#include "Texture2D.h"

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
				return std::make_shared<OpenGLTexture2D>(path);
		}

		ASSERT(false, "Unknown renderer API.");
		return nullptr;
	}
}