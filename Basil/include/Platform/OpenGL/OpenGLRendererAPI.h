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
			void setClearColor(const glm::vec4& color) override;
			void clear() override;
			void drawIndexed(const Shared<VertexArray>& vao) override;
	};
}
