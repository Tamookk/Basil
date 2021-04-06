#include "pch.h"
#include "RendererAPI.h"

namespace Basil
{
	// Set the renderer API
	RendererAPI::API RendererAPI::api = RendererAPI::API::OpenGL;

	// Get the API in use
	RendererAPI::API RendererAPI::getAPI()
	{
		return api;
	}
}