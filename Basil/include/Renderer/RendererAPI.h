/*
 * This header file declares an interface for the renderer that will be implemented per-platform.
 */
#pragma once

#include "Renderer/VertexArray.h"

#include <glm/glm.hpp>

namespace Basil
{
	class RendererAPI
	{
		public:
			enum class API
			{
				None = 0,
				OpenGL = 1
			};
			virtual void init() = 0;
			virtual void setViewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height) = 0;
			virtual void setClearColor(const glm::vec4& color) = 0;
			virtual void clear() = 0;
			virtual void drawIndexed(const Shared<VertexArray>& vao, uint32_t count = 0) = 0;
			virtual void drawLines(const Shared<VertexArray>& vao, uint32_t vertexCount) = 0;
			virtual void setLineWidth(float width) = 0;
			static API getAPI();
			static Unique<RendererAPI> create();
		private:
			static API api;
	};
}