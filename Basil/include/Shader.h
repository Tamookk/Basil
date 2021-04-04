/*
 * This header file declares a class for creating an OpenGL shader.
 */
#pragma once

#include <string>

namespace Basil
{
	class Shader
	{
		public:
			Shader(const std::string& vertexShaderSource, const std::string& fragmentShaderSource);
			~Shader();
			void bind() const;
			void unbind() const;
		private:
			uint32_t rendererID;
	};
}
