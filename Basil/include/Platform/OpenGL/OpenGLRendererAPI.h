/*
 * This header file declares the OpenGL-specific implementation of RendererAPI.
 */
#pragma once

#include "Renderer/RendererAPI.h"

namespace Basil
{
	class OpenGLRendererAPI : public RendererAPI
	{
		public:
			void init() override;
			void setViewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height) override;
			void setClearColor(const glm::vec4& color) override;
			void clear() override;
			void drawIndexed(const Shared<VertexArray>& vao, uint32_t indexCount = 0) override;
			void drawLines(const Shared<VertexArray>& vao, uint32_t vertexCount) override;
			void setLineWidth(float width) override;
	};
}
