#include "pch.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"

#include <glad/glad.h>

namespace Basil
{
	// A callback for logging OpenGL messages
	void OpenGLMessageCallback(unsigned source, unsigned type, unsigned id, unsigned severity, int length, const char* message, const void* userParam)
	{
		switch (severity)
		{
			case GL_DEBUG_SEVERITY_HIGH:
				LOG_FATAL(message);
				return;
			case GL_DEBUG_SEVERITY_MEDIUM:
				LOG_ERROR(message);
				return;
			case GL_DEBUG_SEVERITY_LOW:
				LOG_WARN(message);
				return;
			case GL_DEBUG_SEVERITY_NOTIFICATION:
				LOG_TRACE(message);
				return;
		}
	}

	// Initialise the renderer
	void OpenGLRendererAPI::init()
	{
		PROFILE_FUNCTION();

		// Setup OpenGL logging if running in debug mode
		#ifdef DEBUG
				glEnable(GL_DEBUG_OUTPUT);
				glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
				glDebugMessageCallback(OpenGLMessageCallback, nullptr);
				glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
		#endif

		// Enable texture blending
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_DEPTH_TEST);
	}

	// Change the viewport
	void OpenGLRendererAPI::setViewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height)
	{
		glViewport(x, y, width, height);
	}

	// Set the clear colour
	void OpenGLRendererAPI::setClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	// Clear the screen
	void OpenGLRendererAPI::clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	// Draw the object(s) in the VAO
	void OpenGLRendererAPI::drawIndexed(const Shared<VertexArray>& vao)
	{
		glDrawElements(GL_TRIANGLES, vao->getIndexBuffer()->getSize(), GL_UNSIGNED_INT, nullptr);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}