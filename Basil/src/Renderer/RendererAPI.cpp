#include "pch.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"
#include "Renderer/RendererAPI.h"

namespace Basil
{
	// Set the renderer API
	RendererAPI::API RendererAPI::api = RendererAPI::API::OpenGL;

	// Get the API in use
	RendererAPI::API RendererAPI::getAPI()
	{
		return api;
	}

	// Create the renderer API
	Unique<RendererAPI> RendererAPI::create()
	{
		switch (api)
		{
		case RendererAPI::API::None:
			ASSERT(false, "Unknown renderer API.");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return makeUnique<OpenGLRendererAPI>();
		}

		ASSERT(false, "Unknown renderer API.");
		return nullptr;
	}
}