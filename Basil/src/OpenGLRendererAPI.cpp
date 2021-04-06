#include "pch.h"
#include "OpenGLRendererAPI.h"

#include <glad/glad.h>

namespace Basil
{
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
	void OpenGLRendererAPI::drawIndexed(const std::shared_ptr<VertexArray>& vao)
	{
		glDrawElements(GL_TRIANGLES, vao->getIndexBuffer()->getSize(), GL_UNSIGNED_INT, nullptr);
	}
}