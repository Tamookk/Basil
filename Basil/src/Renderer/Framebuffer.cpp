#include "pch.h"
#include "Renderer/Framebuffer.h"
#include "Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLFramebuffer.h"

namespace Basil
{
	// Create a framebuffer instance
	Shared<Framebuffer> Framebuffer::create(const FramebufferSpecification& spec)
	{
		switch (Renderer::getAPI())
		{
			case RendererAPI::API::None:
				ASSERT(false, "No renderer API!");
				return nullptr;
			case RendererAPI::API::OpenGL:
				return makeShared<OpenGLFramebuffer>(spec);
		}

		ASSERT(false, "Unknown renderer API.");
		return nullptr;
	}
}