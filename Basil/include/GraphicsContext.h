/*
 * This header defines the rendering context, which is a parent class for API-specific graphics contexts.
 */
#pragma once

namespace Basil
{
	class GraphicsContext
	{
		public:
			virtual void init() = 0;
			virtual void swapBuffers() = 0;
	};
}