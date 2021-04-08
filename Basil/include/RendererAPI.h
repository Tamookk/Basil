/*
 * This header file declares an interface for the renderer that will be implemented per-platform.
 */
#pragma once

#include "VertexArray.h"

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
			virtual void setClearColor(const glm::vec4& color) = 0;
			virtual void clear() = 0;
			virtual void drawIndexed(const std::shared_ptr<VertexArray>& vao) = 0;
			static API getAPI();
		private:
			static API api;
	};
}