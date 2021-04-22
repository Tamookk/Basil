#include "pch.h"
#include "Platform/OpenGL/OpenGLContext.h"
#include "Renderer/GraphicsContext.h"
#include "Renderer/Renderer.h"

namespace Basil
{
	Unique<GraphicsContext> GraphicsContext::create(void* window)
	{
		switch (Renderer::getAPI())
		{
		case RendererAPI::API::None:
			ASSERT(false, "No renderer API.");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return makeUnique<OpenGLContext>(static_cast<GLFWwindow*>(window));
		}

		ASSERT(false, "No renderer API.");
		return nullptr;
	}
}