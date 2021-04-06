/*
 * This header file declares the OpenGL-specific implementation of RendererAPI
 */
#pragma once

#include "RendererAPI.h"

namespace Basil
{
	class OpenGLRendererAPI : public RendererAPI
	{
		public:
			void setClearColor(const glm::vec4& color) override;
			void clear() override;
			void drawIndexed(const std::shared_ptr<VertexArray>& vao) override;
	};
}
