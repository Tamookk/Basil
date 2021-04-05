/*
 * This header decleares a class used for getting the graphics API currently in use.
 */
#pragma once
namespace Basil
{
	enum class RendererAPI
	{
		None = 0,
		OpenGL = 1
	};

	class Renderer
	{
		public:
			static void setAPI(RendererAPI api);
			static RendererAPI getAPI();
		private:
			static RendererAPI rendererAPI;
	};
}