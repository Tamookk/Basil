/*
 * This class defines the OpenGL-specific graphics context.
 */
#pragma once

#include "GraphicsContext.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"

struct GLFWwindow;

namespace Basil
{
	class OpenGLContext : public GraphicsContext
	{
		public:
			OpenGLContext(GLFWwindow* window);
			void init() override;
			void swapBuffers() override;
		private:
			GLFWwindow* window;
	};
}